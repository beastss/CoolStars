package org.cocos2dx.lib;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;

import com.umeng.analytics.game.UMGameAgent;

import android.content.Context;
import android.util.Log;
import android.util.Xml;
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
	
	class ItemData {
		double cost;
		String name;

		void setCost(double c) {
			cost = c;
		}

		void setName(String n) {
			name = n;
		}
		
		double getCost() {
			return cost;
		}

		String getName() {
			return name;
		}
	}

	List<ItemData> items = new ArrayList<ItemData>();

	String[] toastText = { "��ʯ����", "���ϲ���", "��������" };

	private void initItemData() {
		InputStream is = null;
		try {
			is = mContext.getResources().getAssets().open("billing.xml");
		} catch (Exception e) {
			Log.e("test", "open xml errer!!");
			e.printStackTrace();
		}

		ItemData item = null;
		XmlPullParser parser = Xml.newPullParser();
		try {
			parser.setInput(is, "gbk");
			int eventCode = parser.getEventType();
			while (eventCode != XmlPullParser.END_DOCUMENT) {

				switch (eventCode) {
				case XmlPullParser.START_DOCUMENT:
					break;
				case XmlPullParser.START_TAG:
					if ("item".equals(parser.getName())) {
						// ͨ��getName�ж϶����ĸ���ǩ��Ȼ��ͨ��nextText()��ȡ�ı��ڵ�ֵ����ͨ��getAttributeValue(i)��ȡ���Խڵ�ֵ
						item = new ItemData();
						item.setName(parser.getAttributeValue(0));
						item.setCost(Double.valueOf(parser.getAttributeValue(3)));
						items.add(item);
					}
					break;
				case XmlPullParser.END_TAG:
					break;
				}
				eventCode = parser.next();
			}

		} catch (Exception e) {
			Log.e("test", "parse xml fail!!");
			e.printStackTrace();
		}
	}

	// itemId �Ǵ�1��ʼ
	int curItemId;
	public void purchase(int itemId) {
		Log.e("test", "into here!!");
		if (itemId < 1 || itemId > items.size()) {
			return;
		}

		curItemId = itemId - 1;
		ItemData item = items.get(curItemId);
		
		Toast.makeText(mContext, "����ɹ�", Toast.LENGTH_SHORT).show();
		onPurchase(PAY_RESULT_SUCCESS);
		
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