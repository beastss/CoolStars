package org.cocos2dx.lib;

import java.security.MessageDigest;

import com.umeng.analytics.game.UMGameAgent;

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

	public static PayAndroidApi actInstance = null;// 定义单例
	public static Context mContext = null;// 定义单例
	private static boolean mbDebug = true;
	protected static Handler sMainThreadHandler = null;
	private DongXinPush mPush = null;
	public static Object rtnActivity() {
		return actInstance;
	}

	ExternSdkInterface sdkObj;

	public PayAndroidApi(Context context) {
		mContext = context;
		actInstance = this;
		if (null == sMainThreadHandler) {
			sMainThreadHandler = new Handler();
		}
		init();
	}

	private void init() {
		sdkObj = new TemplateSdk(mContext);
		mPush = new DongXinPush(mContext);
	}

	public void Purchase(final int itemId) {
		runOnMainThread(new Runnable() {
			@Override
			public void run() {
				sdkObj.purchase(itemId);
			}
		});

	}

	public void exitGame() {
		sdkObj.exitGame();
	}
	
	public void startGame()
	{
		sdkObj.startGame();
	}

	public static void runOnMainThread(Runnable r) {
		if (null == sMainThreadHandler)
			return;
		sMainThreadHandler.post(r);
	}
	
	public void showToast(final int toastIndex) {
		runOnMainThread(new Runnable() {
			@Override
			public void run() {
				String text = sdkObj.getTextByIndex(toastIndex);
				Toast.makeText(mContext, text, Toast.LENGTH_LONG).show();
			}
		});
	}

	public static void showToast(final String toastText){
		runOnMainThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(mContext, toastText, Toast.LENGTH_LONG).show();
			}
		});
	}
	
	public void startStage(int stage)
	{
		Log.d(LOG_TAG, "startStage....");
		String str = "level_";
		str += stage;
		UMGameAgent.startLevel(str);
	}
	
	public void endStage(int stage, boolean win)
	{
		Log.d(LOG_TAG, "startStage...d");
		String str = "level_";
		str += stage;
		if(win)
		{
			UMGameAgent.finishLevel(str);
			UMGameAgent.setPlayerLevel(stage);
		}
		else
		{
			UMGameAgent.failLevel(str);
		}
	}
	
	public void buyItemByDiamond(String name, int cost)
	{
		Log.d("analysis", String.format("buyItemByDiamond name: %s cost: %d", name, cost));
		UMGameAgent.buy(name, 1, cost); 
	}
	
	public void useItem(String name)
	{
		Log.d("analysis", String.format("useItem name: %s", name));
		UMGameAgent.use(name, 1 , 0);
	}
	
	public boolean isForBusiness()
	{
		return mPush.isBusinessMode();
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
