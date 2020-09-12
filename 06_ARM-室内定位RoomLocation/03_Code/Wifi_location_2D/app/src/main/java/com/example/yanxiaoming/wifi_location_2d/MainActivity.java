package com.example.yanxiaoming.wifi_location_2d;

import android.net.wifi.ScanResult;
import android.net.wifi.WifiManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import java.util.List;

import Jama.Matrix;

import static Jama.examples.MagicSquareExample.fixedWidthDoubletoString;
import static java.lang.Math.pow;

public class MainActivity extends AppCompatActivity {

    double d1=0,d2=0,d3=0;
    double dp1=0,dp2=0,dp3=0;
    int flag=0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void GetWifiRssi(View view) {

        String SSID1="FaryLink_4F2BAD";
        String SSID2="AI-THINKER_1BD050";
        String SSID3="002713_ESP8266";

        EditText editText = (EditText) findViewById(R.id.edit_message);
        WifiManager wifiManager=(WifiManager)
                getApplicationContext().getSystemService(WIFI_SERVICE);
        wifiManager.startScan();
        List<ScanResult> scanResults=wifiManager.getScanResults();
        for (ScanResult scanResult : scanResults) {
          //  editText.append("\nSSID: "+scanResult.SSID
           //         +" RSSI: "+scanResult.level+"/n: "
           //         +wifiManager.calculateSignalLevel(scanResult.level,4));

            String SSID;
            SSID= "" + scanResult.SSID;
            if( SSID.equals(SSID1)){
                dp1=scanResult.level;
                d1=pow(10,(-dp1-36.)/40.);
                editText.append("dp1:"+dp1+"\n");
                flag++;
            }

            if( SSID.equals(SSID2)){
                dp2=scanResult.level;
                d2=pow(10,(-dp2-42.)/40.);
                editText.append("dp2:"+dp2+"\n");
                flag++;
            }

            if( SSID.equals(SSID3)){
                dp3=scanResult.level;
                d3=pow(10,(-dp3-39.)/40.);
                editText.append("dp3:"+dp3+"\n");
                flag++;
            }

        }
        if(flag>=3) {
            jamaTest();
            flag=0;
        }


    }
//d = 10^((abs(rssi) - A) / (10 * n))
    public void jamaTest () {

    /*    double d1=0.001441*dp1*dp1*dp1+0.225*dp1*dp1+11.17*dp1+180.2;
        double d2=0.001441*dp2*dp2*dp2+0.225*dp2*dp2+11.17*dp2+180.2;
        double d3=0.001441*dp3*dp3*dp3+0.225*dp3*dp3+11.17*dp3+180.2;*/

        double x1=0.,y1=0.,x2=11*0.92,y2=0.,y3=7*0.92,x3=0.;
        double[][] arrayA={{2*(x1-x2),2*(y1-y2)},{2*(x1-x3),2*(y1-y3)}};
        double[][] arrayB={{(d2*d2-d1*d1+x1*x1-x2*x2+y1*y1-y2*y2)},{(d3*d3-d1*d1+x1*x1-x3*x3+y1*y1-y3*y3)}};
        Matrix A = new Matrix(arrayA);
        Matrix b = new Matrix(arrayB);
        Matrix x = (((A.transpose().times(A)).inverse()).times(A.transpose())).times(b);
        printTest("A:\n");
        for(int i=0;i<2;i++) {
            for(int j=0;j<2;j++) {
                printTest(fixedWidthDoubletoString(A.get(i, j),10,3));
            }
            printTest("\n");
        }
        printTest("b:\n");
        for(int i=0;i<2;i++) {
            printTest(fixedWidthDoubletoString(b.get(i, 0),10,3));
        }
        printTest("\n");
        printTest("x:\n");
        for(int i=0;i<2;i++) {
            printTest(fixedWidthDoubletoString(x.get(i, 0),10,3));
        }

    }

    public void printTest (String s) {
        //
        TextView tv = (TextView) findViewById(R.id.text_info);
        tv.append(s);
    }

    public String fixedWidthDoubletoString (double x, int w, int d) {
        java.text.DecimalFormat fmt = new java.text.DecimalFormat();
        fmt.setMaximumFractionDigits(d);
        fmt.setMinimumFractionDigits(d);
        fmt.setGroupingUsed(false);
        String s = fmt.format(x);
        while (s.length() < w) {
            s = " " + s;
        }
        return s;
    }

    public String fixedWidthIntegertoString (int n, int w) {
        String s = Integer.toString(n);
        while (s.length() < w) {
            s = " " + s;
        }
        return s;
    }
}
