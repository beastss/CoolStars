package org.cocos2dx.lib;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

public class TemplateSdk implements ExternSdkInterface {
	private static final int PAY_RESULT_SUCCESS = 0;
	private static final int PAY_RESULT_FAILED = 1;
	private static final int PAY_RESULT_CANCEL = 2;
	private static final int PAY_RESULT_TIMEOUT = 3;
	String[] toastText = { "钻石不足", "饲料不足", "体力不足" };
	
	public static Context mContext = null;
	private GameBilling mBilling;

	TemplateSdk(Context context) {
		mContext = context;
		mBilling = new GameBilling(context);
	}

	// itemId 是从1开始
	int curItemId;
	public void purchase(int itemId) {
		curItemId = itemId;
		Log.e("test", "android sdk purchase: " + itemId);
		ItemData data = mBilling.getBillData(itemId);
		Toast.makeText(mContext, " " + data.getName() + "购买成功", Toast.LENGTH_SHORT).show();
		mBilling.onPurchase(PAY_RESULT_SUCCESS);
	}

	public void startGame() {

	}

	public void exitGame() {
		System.exit(0);
	}

	public String getTextByIndex(int index) {
		if (index < toastText.length) {
			return toastText[index];
		} else {
			return "";
		}
	}

}