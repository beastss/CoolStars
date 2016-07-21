package org.cocos2dx.lib;

interface ExternSdkInterface
{
	public void purchase(int itemId);
	public void startGame();
	public void exitGame();
	public String getTextByIndex(int index);
}