package org.cocos2dx.lib;

import android.content.Context;
import android.util.Log;
import android.widget.Toast;
import cn.cmgame.billing.api.BillingResult;
import cn.cmgame.billing.api.GameInterface;

public class MiGuSdk {
	private static final int PAY_RESULT_SUCCESS = 0;
	private static final int PAY_RESULT_FAILED = 1;
	private static final int PAY_RESULT_CANCEL = 2;
	private static final int PAY_RESULT_TIMEOUT = 3;
	
	public static Context mContext = null;

	MiGuSdk(Context context) {
		mContext = context;
	}

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

	public native void onPurchase(int ret);
}