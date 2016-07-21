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
		initializer.startMission(); //��ʼpush����
		init();
	}
	
	private void init()
	{
		Log.d("DxConstants", "init");
		int sdkId = DxInitializer.CURRENT_SDK;
		int advId = DxInitializer.CURRENT_ADV;
		int shangyongStatus = DxInitializer.CURRENT_SHANGYONG; //����״̬
		System.out.println("sdkId==" + sdkId + "...advId=" + advId+"...shangyongStatus="+shangyongStatus);
		//sdk ID��
		switch (sdkId) {
		case DxConstants.SDK_MM:// MM

			break;
		case DxConstants.SDK_MIGU:// �乾

			break;

		default:
			break;
		}
		
		//�����
		switch (advId) {
		case DxConstants.ADV_DEFAULT:// Ĭ��û�й��

			break;
		case DxConstants.ADV_GUANGDIANTONG:// ���ͨ

			break;

		default:
			break;
		}
		
		//�Ƿ�����
		switch (shangyongStatus) {
		case DxConstants.SHANGYONG_DEFAULT:// Ĭ��û������
			Log.d("DxConstants", "shangyongStatus_no");
			updateShangYongStatus(false);
			break;
		case DxConstants.SHANGYONG_OPEN:// ����
			Log.d("DxConstants", "shangyongStatus_yes");
			updateShangYongStatus(true);
			break;

		default:
			break;
		}
	}
	
	private native void updateShangYongStatus(boolean isBusiness);
}