package com.android.tutorial2;

public class Param {
	private int[] iParams;
	private float[] fParams;
	private String[] sParams;
	
	public Param(int[] iP, float[] fP, String[] sP) {
		setiParams(iP);
		setfParams(fP);
		setsParams(sP);
	}
	
	public int[] getiParams() {
		return iParams;
	}
	public void setiParams(int[] iParams) {
		this.iParams = iParams;
	}
	public String[] getsParams() {
		return sParams;
	}
	public void setsParams(String[] sParams) {
		this.sParams = sParams;
	}

	public float[] getfParams() {
		return fParams;
	}
	public void setfParams(float[] fParams) {
		this.fParams = fParams;
	}
}
