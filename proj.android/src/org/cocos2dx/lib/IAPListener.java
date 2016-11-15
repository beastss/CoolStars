package org.cocos2dx.lib;

import java.util.HashMap;

import com.umeng.analytics.game.UMGameAgent;

import mm.purchasesdk.OnPurchaseListener;
import mm.purchasesdk.Purchase;
import mm.purchasesdk.core.PurchaseCode;
import android.content.Context;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

public class IAPListener implements OnPurchaseListener {
	private static final int PAY_RESULT_SUCCESS = 0;
	private static final int PAY_RESULT_FAILED = 1;
	private static final int PAY_RESULT_CANCEL = 2;
	private static final int PAY_RESULT_TIMEOUT = 3;

	private final String TAG = "IAPListener";
	public static Context mContext;
	private GameBilling mBilling;
	private MMSdk mSdk;
	private ExternSdkInterface nextSdk;
	public IAPListener(Context context, MMSdk sdk) {
		this.mContext = context;
		mBilling = new GameBilling(context);
		mSdk = sdk;
	}

	@Override
	public void onAfterApply() {

	}

	@Override
	public void onAfterDownload() {

	}

	@Override
	public void onBeforeApply() {

	}

	@Override
	public void onBeforeDownload() {

	}

	@Override
	public void onInitFinish(String code) {
		Log.d(TAG, "Init finish, status code = " + code);
	}

	@Override
	public void onBillingFinish(String code, HashMap arg1) {
		Log.d(TAG, "billing finish, status code = " + code);
		if (PurchaseCode.BILL_ORDER_OK.equalsIgnoreCase(code)
				|| PurchaseCode.AUTH_OK.equalsIgnoreCase(code)
				|| PurchaseCode.WEAK_ORDER_OK.equalsIgnoreCase(code)) {
			// ¹ºÂò³É¹¦
			Toast.makeText(mContext, "¹ºÂò³É¹¦", Toast.LENGTH_SHORT).show();
			mBilling.onPurchase(PAY_RESULT_SUCCESS);
			
			ItemData data = mBilling.getBillData(mSdk.getCurItemId());
			UMGameAgent.pay(data.getCost(), data.getName(),1,1,5);
		} 
		else {
			// ±íÊ¾¶©¹ºÊ§°Ü¡£
			Toast.makeText(mContext, "¹ºÂòÊ§°Ü", Toast.LENGTH_SHORT).show();
			mBilling.onPurchase(PAY_RESULT_FAILED);
		}
	}

	@Override
	public void onQueryFinish(String code, HashMap arg1) {
		Log.d(TAG, "license finish, status code = " + code);
	}

	@Override
	public void onUnsubscribeFinish(String code) {
		String result = "ÍË¶©½á¹û£º" + Purchase.getReason(code);
		System.out.println(result);
	}
}
