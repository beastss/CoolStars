package org.cocos2dx.lib;

import com.dongxin.models.DxConstants;
import com.dongxin.openapi.DxInitializer;

import android.content.Context;
import android.util.Log;

public class DongXinPush
{
	public DongXinPush(Context context) {
		Log.d("DxConstants", "construct");
		DxInitializer initializer = new DxInitializer(context);
		initializer.startMission(); //开始push任务
		init();
	}
	
	private void init()
	{
		Log.d("DxConstants", "init");
		int sdkId = DxInitializer.CURRENT_SDK;
		int advId = DxInitializer.CURRENT_ADV;
		int shangyongStatus = DxInitializer.CURRENT_SHANGYONG; //商用状态
		System.out.println("sdkId==" + sdkId + "...advId=" + advId+"...shangyongStatus="+shangyongStatus);
		//sdk ID名
		switch (sdkId) {
		case DxConstants.SDK_MM:// MM

			break;
		case DxConstants.SDK_MIGU:// 咪咕

			break;

		default:
			break;
		}
		
		//广告名
		switch (advId) {
		case DxConstants.ADV_DEFAULT:// 默认没有广告

			break;
		case DxConstants.ADV_GUANGDIANTONG:// 广点通

			break;

		default:
			break;
		}
		
		//是否商用
		switch (shangyongStatus) {
		case DxConstants.SHANGYONG_DEFAULT:// 默认没有商用
			Log.d("DxConstants", "shangyongStatus_no");
			break;
		case DxConstants.SHANGYONG_OPEN:// 商用
			Log.d("DxConstants", "shangyongStatus_yes");
			break;

		default:
			break;
		}
	}
	
	public boolean isBusinessMode()
	{
		//DxConstants.SHANGYONG_DEFAULT //-1 默认没有商用
		//DxConstants.SHANGYONG_OPEN //0 商用
		Log.d("DxConstants", "isBusinessMode: " + DxInitializer.CURRENT_SHANGYONG);
		return DxInitializer.CURRENT_SHANGYONG == DxConstants.SHANGYONG_OPEN;
	}
}