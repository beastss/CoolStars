package org.cocos2dx.lib;

import java.security.MessageDigest;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Handler;
import android.telephony.TelephonyManager;
import android.util.Base64;
import android.util.Log;
import android.widget.Toast;

public class PayAndroidApi {
	private static final String LOG_TAG = "PayAndroidApi";
	String[] toastText = 
		{"钻石不足",
		"饲料不足",
		"体力不足"};  

	public static PayAndroidApi actInstance = null;// 定义单例
	public static Context mContext = null;// 定义单例
	private static boolean mbDebug = true;
	protected static Handler sMainThreadHandler = null;

	public static Object rtnActivity() {
		return actInstance;
	}

	MiGuSdk migu;

	public PayAndroidApi(Context context) {
		mContext = context;
		actInstance = this;
		init();
		if (null == sMainThreadHandler) {
			sMainThreadHandler = new Handler();
		}
	}

	private void init() {
		migu = new MiGuSdk(mContext);
	}

	int itemId = 0;

	public void Purchase(int id) {
		itemId = id;
		runOnMainThread(new Runnable() {
			@Override
			public void run() {
				migu.purchase(itemId);
			}
		});

	}

	private void exitGame() {
		migu.exitGame();
	}

	public static void runOnMainThread(Runnable r) {
		if (null == sMainThreadHandler)
			return;
		sMainThreadHandler.post(r);
	}
	
	int toastIndex = 0;
	public void showToast(int index) {
		toastIndex = index;
		runOnMainThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(mContext, toastText[toastIndex], Toast.LENGTH_LONG).show();
			}
		});
	}

	public String getVerName() {
		String version = "V1.0.0";
		try {
			PackageManager manager = mContext.getPackageManager();
			PackageInfo info = manager.getPackageInfo(
					mContext.getPackageName(), 0);
			version = info.versionName;
			return version;
		} catch (Exception e) {
			e.printStackTrace();
		}
		return version;
	}

	protected static void LogD(String msg) {
		if (mbDebug) {
			Log.d(LOG_TAG, msg);
		}
	}

	protected void LogE(String msg) {
		if (mbDebug) {
			Log.e(LOG_TAG, msg);
		}
	}

}
