package com.android.tutorial2;

public class Param {
	private int[] iParams;
	private long[] lParams;
	private String[] sParams;
	
	public Param(int[] iP) {
		setiParams(iP);
	}
	
	public int[] getiParams() {
		return iParams;
	}
	public void setiParams(int[] iParams) {
		this.iParams = iParams;
	}
	public long[] getlParams() {
		return lParams;
	}
	public void setlParams(long[] lParams) {
		this.lParams = lParams;
	}
	public String[] getsParams() {
		return sParams;
	}
	public void setsParams(String[] sParams) {
		this.sParams = sParams;
	}
}
