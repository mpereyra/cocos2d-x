/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.lib;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.inputmethod.InputMethodManager;

import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGL10;

public class Cocos2dxGLSurfaceView extends GLSurfaceView {

  /* BPC EGL Config Chooser */
  public class BPCEGLChooser implements GLSurfaceView.EGLConfigChooser {
    @Override
    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) {
          int[] oValue = new int[1];
          int[] configSpec = {
                  EGL10.EGL_RED_SIZE, 8,
                  EGL10.EGL_GREEN_SIZE, 8,
                  EGL10.EGL_BLUE_SIZE, 8,
                  EGL10.EGL_ALPHA_SIZE, 8,
                  EGL10.EGL_RENDERABLE_TYPE, 4,//EGL10.EGL_OPENGL_ES2_BIT
                  EGL10.EGL_NONE
          };

          if (!egl.eglChooseConfig(display, configSpec, null, 0, oValue)) {
              throw new IllegalArgumentException("eglChooseConfig failed");
          }

          int numConfigs = oValue[0];
          if (numConfigs <= 0) {
              throw new IllegalArgumentException("No config matches found");
          }

          EGLConfig[] configs = new EGLConfig[numConfigs];
          if (!egl.eglChooseConfig(display, configSpec, configs, numConfigs, oValue)) {
              throw new IllegalArgumentException("eglChooseConfig failed");
          }

          // We're just looking for the best depth we can find
          int best = -1;
          for (int i = 0; i < configs.length; ++i) {
              int depth = findConfigAttrib(egl, display, configs[i], EGL10.EGL_DEPTH_SIZE, 0);

              final StringBuilder sb = new StringBuilder();
              sb.append(depth);
              if (depth == 16 && best == -1) {
                best = i;
              }
              if(depth == 24) {
                best = i;
                break;
              }
          }

          if (best == -1) {
              throw new IllegalArgumentException("No config matches found");
          }

          EGLConfig config = configs.length > 0 ? configs[best] : null;
          if (config == null) {
              throw new IllegalArgumentException("No config matches found");
          }
          return config;
      }

      // Helper
      private int findConfigAttrib(EGL10 egl, EGLDisplay display,
          EGLConfig config, int attribute, int defaultValue) {
       int[] oValue = new int[1];
      if (egl.eglGetConfigAttrib(display, config, attribute, oValue)) {
          return oValue[0];
      }
      return defaultValue;
    }
  }
  /**************************************************************/

	// ===========================================================
	// Constants
	// ===========================================================

	private static final String TAG = Cocos2dxGLSurfaceView.class.getSimpleName();

	private final static int HANDLER_OPEN_IME_KEYBOARD = 2;
	private final static int HANDLER_CLOSE_IME_KEYBOARD = 3;

	// ===========================================================
	// Fields
	// ===========================================================

	// TODO Static handler -> Potential leak!
	private static Handler sHandler;

	private static Cocos2dxGLSurfaceView mCocos2dxGLSurfaceView;
	private static Cocos2dxTextInputWraper sCocos2dxTextInputWraper;

	private Cocos2dxRenderer mCocos2dxRenderer;
	private Cocos2dxEditText mCocos2dxEditText;

	/* BPC PATCH: onTouchEvent is still called when backgrounded
	   if you are dragging/panning while pressing the home key.
	   Setting focus doesn't help, so use this bool on pause/resume. */
	private boolean mHandleTouches = true;
  private EGLConfigChooser mConfigChooser = new BPCEGLChooser();

	// ===========================================================
	// Constructors
	// ===========================================================

	public Cocos2dxGLSurfaceView(final Context context) {
		super(context);

		this.setEGLContextClientVersion(2);
    this.setEGLConfigChooser(mConfigChooser);

		this.initView();
	}

	public Cocos2dxGLSurfaceView(final Context context, final AttributeSet attrs) {
		super(context, attrs);

		this.setEGLContextClientVersion(2);
    this.setEGLConfigChooser(mConfigChooser);

		this.initView();
	}

	protected void initView() {
		this.setFocusableInTouchMode(true);

		Cocos2dxGLSurfaceView.mCocos2dxGLSurfaceView = this;
		Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper = new Cocos2dxTextInputWraper(this);

		Cocos2dxGLSurfaceView.sHandler = new Handler() {
			@Override
			public void handleMessage(final Message msg) {
				switch (msg.what) {
					case HANDLER_OPEN_IME_KEYBOARD:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText && Cocos2dxGLSurfaceView.this.mCocos2dxEditText.requestFocus()) {
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.removeTextChangedListener(Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper);
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.setText("");
							final String text = (String) msg.obj;
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.append(text);
							Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper.setOriginText(text);
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.addTextChangedListener(Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper);
							final InputMethodManager imm = (InputMethodManager) Cocos2dxGLSurfaceView.mCocos2dxGLSurfaceView.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
							imm.showSoftInput(Cocos2dxGLSurfaceView.this.mCocos2dxEditText, 0);
							Log.d("GLSurfaceView", "showSoftInput");
						}
						break;

					case HANDLER_CLOSE_IME_KEYBOARD:
						if (null != Cocos2dxGLSurfaceView.this.mCocos2dxEditText) {
							Cocos2dxGLSurfaceView.this.mCocos2dxEditText.removeTextChangedListener(Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper);
							final InputMethodManager imm = (InputMethodManager) Cocos2dxGLSurfaceView.mCocos2dxGLSurfaceView.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
							imm.hideSoftInputFromWindow(Cocos2dxGLSurfaceView.this.mCocos2dxEditText.getWindowToken(), 0);
							Log.d("GLSurfaceView", "HideSoftInput");
						}
						break;
				}
			}
		};
	}

	// ===========================================================
	// Getter & Setter
	// ===========================================================

	public void setCocos2dxRenderer(final Cocos2dxRenderer renderer) {
		this.mCocos2dxRenderer = renderer;
		this.setRenderer(this.mCocos2dxRenderer);
	}

	private String getContentText() {
		return this.mCocos2dxRenderer.getContentText();
	}

	public Cocos2dxEditText getCocos2dxEditText() {
		return this.mCocos2dxEditText;
	}

	public void setCocos2dxEditText(final Cocos2dxEditText pCocos2dxEditText) {
		this.mCocos2dxEditText = pCocos2dxEditText;
		if (null != this.mCocos2dxEditText && null != Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper) {
			this.mCocos2dxEditText.setOnEditorActionListener(Cocos2dxGLSurfaceView.sCocos2dxTextInputWraper);
			this.mCocos2dxEditText.setCocos2dxGLSurfaceView(this);
			this.requestFocus();
		}
	}

	// ===========================================================
	// Methods for/from SuperClass/Interfaces
	// ===========================================================

	@Override
	public void onResume() {
		super.onResume();

		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleOnResume();
			}
		});
		//BPC PATCH: to resume onTouchEvent
		mHandleTouches = true;
	}

	@Override
	public void onPause() {
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleOnPause();
			}
		});

		super.onPause();
		//BPC PATCH: to interrupt onTouchEvent
		mHandleTouches = false;
	}

	@Override
	public boolean onTouchEvent(final MotionEvent pMotionEvent) {
		//BPC PATCH: manual interruption of onTouchEvent
		if(!mHandleTouches)
			return true;

		// these data are used in ACTION_MOVE and ACTION_CANCEL
		final int pointerNumber = pMotionEvent.getPointerCount();
		final int[] ids = new int[pointerNumber];
		final float[] xs = new float[pointerNumber];
		final float[] ys = new float[pointerNumber];

		for (int i = 0; i < pointerNumber; i++) {
			ids[i] = pMotionEvent.getPointerId(i);
			xs[i] = pMotionEvent.getX(i);
			ys[i] = pMotionEvent.getY(i);
		}

		switch (pMotionEvent.getAction() & MotionEvent.ACTION_MASK) {
			case MotionEvent.ACTION_POINTER_DOWN:
				final int indexPointerDown = pMotionEvent.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
				final int idPointerDown = pMotionEvent.getPointerId(indexPointerDown);
				final float xPointerDown = pMotionEvent.getX(indexPointerDown);
				final float yPointerDown = pMotionEvent.getY(indexPointerDown);

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionDown(idPointerDown, xPointerDown, yPointerDown);
					}
				});
				break;

			case MotionEvent.ACTION_DOWN:
				// there are only one finger on the screen
				final int idDown = pMotionEvent.getPointerId(0);
				final float xDown = xs[0];
				final float yDown = ys[0];

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionDown(idDown, xDown, yDown);
					}
				});
				break;

			case MotionEvent.ACTION_MOVE:
				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionMove(ids, xs, ys);
					}
				});
				break;

			case MotionEvent.ACTION_POINTER_UP:
				final int indexPointUp = pMotionEvent.getAction() >> MotionEvent.ACTION_POINTER_ID_SHIFT;
				final int idPointerUp = pMotionEvent.getPointerId(indexPointUp);
				final float xPointerUp = pMotionEvent.getX(indexPointUp);
				final float yPointerUp = pMotionEvent.getY(indexPointUp);

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionUp(idPointerUp, xPointerUp, yPointerUp);
					}
				});
				break;

			case MotionEvent.ACTION_UP:
				// there are only one finger on the screen
				final int idUp = pMotionEvent.getPointerId(0);
				final float xUp = xs[0];
				final float yUp = ys[0];

				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionUp(idUp, xUp, yUp);
					}
				});
				break;

			case MotionEvent.ACTION_CANCEL:
				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleActionCancel(ids, xs, ys);
					}
				});
				break;
		}

        /*
		if (BuildConfig.DEBUG) {
			Cocos2dxGLSurfaceView.dumpMotionEvent(pMotionEvent);
		}
		*/
		return true;
	}

	/*
	 * This function is called before Cocos2dxRenderer.nativeInit(), so the
	 * width and height is correct.
	 */
	@Override
	protected void onSizeChanged(final int pNewSurfaceWidth, final int pNewSurfaceHeight, final int pOldSurfaceWidth, final int pOldSurfaceHeight) {
		if(!this.isInEditMode()) {
			this.mCocos2dxRenderer.setScreenWidthAndHeight(pNewSurfaceWidth, pNewSurfaceHeight);
		}
	}

	@Override
	public boolean onKeyDown(final int pKeyCode, final KeyEvent pKeyEvent) {
		switch (pKeyCode) {
			case KeyEvent.KEYCODE_BACK:
			case KeyEvent.KEYCODE_MENU:
				this.queueEvent(new Runnable() {
					@Override
					public void run() {
						Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleKeyDown(pKeyCode);
					}
				});
				return true;
			default:
				return super.onKeyDown(pKeyCode, pKeyEvent);
		}
	}

	// ===========================================================
	// Methods
	// ===========================================================

	// ===========================================================
	// Inner and Anonymous Classes
	// ===========================================================

	public static void openIMEKeyboard() {
		final Message msg = new Message();
		msg.what = Cocos2dxGLSurfaceView.HANDLER_OPEN_IME_KEYBOARD;
		msg.obj = Cocos2dxGLSurfaceView.mCocos2dxGLSurfaceView.getContentText();
		Cocos2dxGLSurfaceView.sHandler.sendMessage(msg);
	}

	public static void closeIMEKeyboard() {
		final Message msg = new Message();
		msg.what = Cocos2dxGLSurfaceView.HANDLER_CLOSE_IME_KEYBOARD;
		Cocos2dxGLSurfaceView.sHandler.sendMessage(msg);
	}

	public void insertText(final String pText) {
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleInsertText(pText);
			}
		});
	}

	public void deleteBackward() {
		this.queueEvent(new Runnable() {
			@Override
			public void run() {
				Cocos2dxGLSurfaceView.this.mCocos2dxRenderer.handleDeleteBackward();
			}
		});
	}

	private static void dumpMotionEvent(final MotionEvent event) {
		final String names[] = { "DOWN", "UP", "MOVE", "CANCEL", "OUTSIDE", "POINTER_DOWN", "POINTER_UP", "7?", "8?", "9?" };
		final StringBuilder sb = new StringBuilder();
		final int action = event.getAction();
		final int actionCode = action & MotionEvent.ACTION_MASK;
		sb.append("event ACTION_").append(names[actionCode]);
		if (actionCode == MotionEvent.ACTION_POINTER_DOWN || actionCode == MotionEvent.ACTION_POINTER_UP) {
			sb.append("(pid ").append(action >> MotionEvent.ACTION_POINTER_ID_SHIFT);
			sb.append(")");
		}
		sb.append("[");
		for (int i = 0; i < event.getPointerCount(); i++) {
			sb.append("#").append(i);
			sb.append("(pid ").append(event.getPointerId(i));
			sb.append(")=").append((int) event.getX(i));
			sb.append(",").append((int) event.getY(i));
			if (i + 1 < event.getPointerCount()) {
				sb.append(";");
			}
		}
		sb.append("]");
		Log.d(Cocos2dxGLSurfaceView.TAG, sb.toString());
	}
}
