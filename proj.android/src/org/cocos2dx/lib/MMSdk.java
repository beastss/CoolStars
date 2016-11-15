package org.cocos2dx.lib;

import mm.purchasesdk.OnPurchaseListener;
import mm.purchasesdk.Purchase;
import mm.purchasesdk.core.ui.PurchaseSkin;
import android.content.Context;
import android.util.Log;


public class MMSdk implements ExternSdkInterface{
	
	private static final String APPID = "300009792285"; // 请填写好正确的值
	private static final String APPKEY = "57D95BBF23CD941E3EFC32A69C7D1678";// 请填写好正确的值
	private OnPurchaseListener mListener;
	public static Purchase purchase;
	private GameBilling mBilling;
	
	public static Context mContext = null;
	MMSdk(Context context) {
		mContext = context;
		mBilling = new GameBilling(context);
		mListener = new IAPListener(mContext, this);
		purchase = Purchase.getInstance();
		try {
			purchase.setAppInfo(APPID, APPKEY, PurchaseSkin.SKIN_SYSTEM_ONE);
		} catch (Exception e1) {
			e1.printStackTrace();
		}
		try {
			purchase.init(context, mListener);

		} catch (Exception e) {
			e.printStackTrace();
			return;
		}	
	}

	String[] toastText = { "钻石不足", "饲料不足", "体力不足" };

	// itemId 是从1开始
	int curItemId;
	public int getCurItemId(){
		return curItemId;
	}
	
	public void purchase(int itemId) {
		curItemId = itemId;
		Log.e("test", "into here!!");
		ItemData data = mBilling.getBillData(itemId);
		
		try {
			String tradeid = purchase.order(mContext, data.getCode(), mListener);
		} catch (Exception e1) {
			e1.printStackTrace();
		}
		
	}

	public void startGame()
	{
		
	}
	
	public void exitGame() {
		System.exit(0);
	}

	public String getTextByIndex(int index)
	{
		if(index < toastText.length)
		{
			return toastText[index];
		}
		else
		{
			return "";
		}
		
	}
	
}