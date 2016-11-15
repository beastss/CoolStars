package com.dongxin.CoolStars;

import android.app.Application;
import com.secneo.mmb.Helper;
import android.content.Context;

public class MyApplication extends Application {
	protected void attachBaseContext(Context ctx) {
		super.attachBaseContext(ctx);
		Helper.install(this);
	}
}

