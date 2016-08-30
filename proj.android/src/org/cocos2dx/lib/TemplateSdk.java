package org.cocos2dx.lib;

import com.umeng.analytics.game.UMGameAgent;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;

public class TemplateSdk implements ExternSdkInterface{
	private static final int PAY_RESULT_SUCCESS = 0;
	private static final int PAY_RESULT_FAILED = 1;
	private static final int PAY_RESULT_CANCEL = 2;
	private static final int PAY_RESULT_TIMEOUT = 3;
	
	public static Context mContext = null;

	TemplateSdk(Context context) {
		mContext = context;
		initItemData();
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
			"推荐宠物包"};
		double[]costs = {
				2,
				8,
				15,
				20,
				30,
				30,
				2,
				0.1,
				30,
		};
		for(int i = 0; i < names.length; ++i)
		{
			items[i] = new ItemData(costs[i], names[i]);
		}
	}

	//itemId 是从1开始
	public void purchase(int itemId) {
		Toast.makeText(mContext, "购买成功", Toast.LENGTH_SHORT).show();
		onPurchase(PAY_RESULT_SUCCESS);
		
	}

	private String getBillingIndex(int i) {
		if (i < 9) {
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
	
	public native void onPurchase(int ret);
}