package org.cocos2dx.lib;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import org.xmlpull.v1.XmlPullParser;
import android.content.Context;
import android.util.Log;
import android.util.Xml;

class ItemData {
	double cost;
	String name;
	String code;

	void setCost(double c) {cost = c;}
	void setName(String n) {name = n;}
	void setCode(String c) {code = c;}
	double getCost() {return cost;}
	String getName() {return name;}
	String getCode() {return code;}
}

public class GameBilling 
{
	public static Context mContext = null;

	GameBilling(Context context) {
		mContext = context;
		initItemData();
	}
	
	List<ItemData> items = new ArrayList<ItemData>();
	
	private void initItemData() {
		InputStream is = null;
		try {
			is = mContext.getResources().getAssets().open("data/billing.xml");
		} catch (Exception e) {
			Log.e("test", "open xml errer!!");
			e.printStackTrace();
		}

		ItemData item = null;
		XmlPullParser parser = Xml.newPullParser();
		try {
			parser.setInput(is, "utf-8");
			int eventCode = parser.getEventType();
			while (eventCode != XmlPullParser.END_DOCUMENT) {

				switch (eventCode) {
				case XmlPullParser.START_DOCUMENT:
					break;
				case XmlPullParser.START_TAG:
					if ("item".equals(parser.getName())) {
						// 通过getName判断读到哪个标签，然后通过nextText()获取文本节点值，或通过getAttributeValue(i)获取属性节点值
						item = new ItemData();
						item.setName(parser.getAttributeValue(0));
						item.setCost(Double.valueOf(parser.getAttributeValue(3)));
						item.setCode(parser.getAttributeValue(4));
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
	
	ItemData getBillData(int itemId)
	{
		itemId -= 1;
		Log.e("test", "ItemData size:............." + items.size());
		if (itemId < 0 || itemId >= items.size()) {
			return null;
		}
		
		return items.get(itemId);
	}
	
	public native void onPurchase(int ret);
}
