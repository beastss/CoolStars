package org.cocos2dx.lib;

import android.app.Activity;
import android.content.Context;
import android.widget.Toast;
import cn.cmgame.billing.api.BillingResult;
import cn.cmgame.billing.api.GameInterface;

import com.umeng.analytics.game.UMGameAgent;

public class MiGuSdk implements ExternSdkInterface{
	private static final int PAY_RESULT_SUCCESS = 0;
	private static final int PAY_RESULT_FAILED = 1;
	private static final int PAY_RESULT_CANCEL = 2;
	private static final int PAY_RESULT_TIMEOUT = 3;
	
	public static Context mContext = null;

	MiGuSdk(Context context) {
		mContext = context;
		initItemData();
		GameInterface.initializeApp((Activity) mContext);
	}
	
	class ItemData
	{
		double cost;
		String name;
		ItemData(double c, String n){cost = c; name = n;}
	}
	ItemData items[] = new ItemData[9];
	
	String[] toastText = 
		{"钻石不足",
		"饲料不足",
		"体力不足"};  
	private void initItemData(){
		String[]names = {
			"200钻石",
			"960钻石",
			"2250钻石",
			"3600钻石",
			"6000钻石",
			"道具礼包",
			"接着玩",
			"新手宠物包",
			"一键开箱"};
		double[]costs = {
				2,
				8,
				15,
				20,
				30,
				30,
				2,
				0.1,
				6
		};
		for(int i = 0; i < names.length; ++i)
		{
			items[i] = new ItemData(costs[i], names[i]);
		}
	}
	
	GameInterface.IPayCallback payCallback = new GameInterface.IPayCallback() {
		@Override
		public void onResult(int resultCode, String billingIndex, Object obj) {			
			String result = "";
			int payResult = PAY_RESULT_SUCCESS;
			switch (resultCode) {
			case BillingResult.SUCCESS:
				if ((BillingResult.EXTRA_SENDSMS_TIMEOUT + "").equals(obj
						.toString())) {
					result = "短信计费超时";
					payResult = PAY_RESULT_TIMEOUT;
				} else {
					result = "购买道具：[" + billingIndex + "] 成功！";
					payResult = PAY_RESULT_SUCCESS;
					int itemId = getItemIndex(billingIndex);
					if(itemId > 0 && itemId <= items.length)
					{
						ItemData data = items[itemId - 1];
						UMGameAgent.pay(data.cost ,data.name, 1, 1, 5);
					}		
				}
				break;
			case BillingResult.FAILED:
				result = "购买道具：[" + billingIndex + "] 失败！";
				payResult = PAY_RESULT_FAILED;
				break;
			default:
				result = "购买道具：[" + billingIndex + "] 取消！";
				payResult = PAY_RESULT_CANCEL;
				break;
			}
			//Toast.makeText(mContext, result, Toast.LENGTH_SHORT).show();
			onPurchase(payResult);
		}
	};

	//itemId 是从1开始
	public void purchase(int itemId) {
		// 计费结果的监听处理，合作方通常需要在收到SDK返回的onResult时，告知用户的购买结果
		String billingIndex = getBillingIndex(itemId);
		GameInterface.doBilling(mContext, true, true, billingIndex, null,
				payCallback);
	}

	private String getBillingIndex(int i) {
		if (i < 10) {
			return "00" + i;
		} else {
			return "0" + i;
		}
	}
	private int getItemIndex(String billingIndex)
	{
		int index = Integer.parseInt(billingIndex);
		return index;
	}

	public void startGame()
	{
		
	}
	
	public void exitGame() {
		GameInterface.exit(mContext, new GameInterface.GameExitCallback() {
			@Override
			public void onConfirmExit() {
				// mContext.finish();
				System.exit(0);
			}

			@Override
			public void onCancelExit() {
				// Toast.makeText(MainActivity.this, "取消退出",
				// Toast.LENGTH_SHORT).show();
			}
		});
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
	
	public native void onPurchase(int ret);
}