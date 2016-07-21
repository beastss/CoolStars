package org.cocos2dx.lib;

import com.umeng.analytics.game.UMGameAgent;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;
import cn.cmgame.billing.api.BillingResult;
import cn.cmgame.billing.api.GameInterface;

public class MiGuSdk implements ExternSdkInterface{
	private static final int PAY_RESULT_SUCCESS = 0;
	private static final int PAY_RESULT_FAILED = 1;
	private static final int PAY_RESULT_CANCEL = 2;
	private static final int PAY_RESULT_TIMEOUT = 3;
	
	String[] itemName =
		{
			"200��ʯ",
			"960��ʯ",
			"2250��ʯ",
			"3600��ʯ",
			"6000��ʯ",
			"�������",
			"������",
			"���ֳ����",
			"�Ƽ������"
		};
		
	String[] toastText = 
		{"��ʯ����",
		"���ϲ���",
		"��������"};  
	
	public static Context mContext = null;

	MiGuSdk(Context context) {
		mContext = context;
	}

	//itemId �Ǵ�1��ʼ
	public void purchase(int itemId) {
		// �Ʒѽ���ļ�������������ͨ����Ҫ���յ�SDK���ص�onResultʱ����֪�û��Ĺ�����
		final GameInterface.IPayCallback payCallback = new GameInterface.IPayCallback() {
			@Override
			public void onResult(int resultCode, String billingIndex, Object obj) {
				String result = "";
				int payResult = PAY_RESULT_SUCCESS;
				switch (resultCode) {
				case BillingResult.SUCCESS:
					if ((BillingResult.EXTRA_SENDSMS_TIMEOUT + "").equals(obj
							.toString())) {
						result = "���żƷѳ�ʱ";
						payResult = PAY_RESULT_TIMEOUT;
					} else {
						result = "������ߣ�[" + billingIndex + "] �ɹ���";
						payResult = PAY_RESULT_SUCCESS;
						int itemId = getItemIndex(billingIndex);
						if(itemId > 0 && itemId <= itemName.length)
						{
							UMGameAgent.pay(10,itemName[itemId], 1, 1, 5);
						}		
					}
					break;
				case BillingResult.FAILED:
					result = "������ߣ�[" + billingIndex + "] ʧ�ܣ�";
					payResult = PAY_RESULT_FAILED;
					break;
				default:
					result = "������ߣ�[" + billingIndex + "] ȡ����";
					payResult = PAY_RESULT_CANCEL;
					break;
				}
				Toast.makeText(mContext, result, Toast.LENGTH_SHORT).show();
				onPurchase(payResult);
			}
		};

		String billingIndex = getBillingIndex(itemId);
		GameInterface.doBilling(mContext, true, true, billingIndex, null,
				payCallback);
	}

	private String getBillingIndex(int i) {
		if (i < 9) {
			return "00" + (++i);
		} else {
			return "0" + (++i);
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
				// Toast.makeText(MainActivity.this, "ȡ���˳�",
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