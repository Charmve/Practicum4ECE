package com.example.yanxiaoming.bluetooth_location_2d;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import java.util.Date;

import Jama.Matrix;

import static java.lang.Math.pow;

public class MainActivity extends AppCompatActivity {

    double dp1=0,dp2=0,dp3=0,dp4=0;     //定义获得的信号强度
    int flag=0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Register the BroadcastReceiver
        IntentFilter filter = new
                IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(mReceiver, filter);

        //jamaTest();
    }

    // Create a BroadcastReceiver for ACTION_FOUND
    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            // When discovery finds a device
            if (BluetoothDevice.ACTION_FOUND.equals(intent.getAction())) {
                // Get the BluetoothDevice object from the Intent
                BluetoothDevice device =
                        intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                // Add the name and address to an array adapter to show in a ListView
                EditText editText = (EditText) findViewById(R.id.edit_message);

                String db;
                db= "" + device.getAddress();     //将获得的mac地址转为字符串
                //已知4个蓝牙模块MAC地址
                String mac1="80:B3:92:D2:BA:97";
                String mac2="80:B3:92:CE:DF:41";
                String mac3="80:B3:92:CE:B0:95";
                String mac4="80:B3:92:D2:E1:2C";
               // editText.append(device.getName() + "\n" + device.getAddress() + "\n"
                      //  + (short) intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI) + "\n");

                //将获得的MAC地址与已知蓝牙模块mac地址比较，相同则把相应的信号强度赋给double 变量，从而得到4个蓝牙模块对应的4个信号强度

                    if( db.equals(mac1) ) {

                        dp1 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                        editText.append(
                                dp1+"dp1"+"\n");flag++;

                    }

                    if(db.equals(mac2)) {

                        dp2 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                        editText.append(
                                dp2+"dp2"+"\n");flag++;
                        // editText.append(
                        // + (short) intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI) + "\n"+dp2+"\n");
                    }

                    if(db.equals(mac4)) {

                        dp3 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                        editText.append(
                                dp3+"dp3"+"\n");flag++;
                    }

                    if(db.equals(mac3)) {

                        dp4 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                        editText.append(
                                dp4+"dp4"+"\n");flag++;
                    }
                if (flag >= 4) {
                    jamaTest();     //将线性代数运算放在按钮函数下，按一次按钮，获取一次蓝牙信号强度，计算一次线性代数
                    flag = 0;
                }
/*
                switch(flag) {
                    case 0:
                        if (db.equals(mac1)) {

                            dp1 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                             editText.append(
                                    dp1+"\n");
                            flag++;
                        }
                        break;
                    case 1:
                        if(db.equals(mac2)) {

                            dp2 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                            editText.append(
                                    dp2+"\n");
                            flag++;
                        }
                        break;
                    case 2:

                            if(db.equals(mac4)) {

                                dp3 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                                editText.append(
                                        dp3+"\n");
                            flag++;
                        }
                        break;
                    case 3:
                        if(db.equals(mac3)) {

                            dp4 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                            editText.append(
                                    dp4+"\n");
                            flag++;
                        }
                        break;
                    default:break;
                }
                */
                /*if( db.equals(mac1) ) {

                     dp1 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                   editText.append(
                            dp1+"\n")
                }

                if(db.equals(mac2)) {

                     dp2 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                   // editText.append(
                           // + (short) intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI) + "\n"+dp2+"\n");
                }

                if(db.equals(mac4)) {

                     dp3 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                }

                if(db.equals(mac3)) {

                     dp4 = intent.getExtras().getShort(BluetoothDevice.EXTRA_RSSI);
                }*/
            }
        }
    };

    public void GetBluetoothRssi(View view) {
        // get Bluetooth adapter
        BluetoothAdapter mBluetoothAdapter =
                BluetoothAdapter.getDefaultAdapter();
        //start discovery
        mBluetoothAdapter.startDiscovery();

    }

    //通过第一个实验测出的距离和强度的对应关系，拟合出两者之间的函数，因为只有7组数据，并且精度太差，所以误差较大
    public double nihe(double db){
        double d,p1,p2,p3,p4;
        p1 = -0.003187;
        p2 = -0.6853 ;
        p3 = -49.05;
        p4 = -1167 ;
        d=  p1*pow(db,3) + p2*pow(db,2) + p3*db + p4;

        return d;
    }

    public void jamaTest () {

      //  double d1,d2,d3,d4;
        double d1=0.0084*dp1*dp1+0.8594*dp1+22.4490;
        double d2=0.0084*dp2*dp2+0.8594*dp2+22.4490;
        double d3=0.0084*dp3*dp3+0.8594*dp3+22.4490;
        double d4=0.0084*dp4*dp4+0.8594*dp4+22.4490;
        //double d1=0.0007007*dp1*dp1*dp1+0.1577*dp1*dp1+11.38*dp1+267.6;
        //double d2=0.0007007*dp2*dp2*dp2+0.1577*dp2*dp2+11.38*dp2+267.6;
       // double d3=0.0007007*dp3*dp3*dp3+0.1577*dp3*dp3+11.38*dp3+267.6;
        //double d4=0.0007007*dp4*dp4*dp4+0.1577*dp4*dp4+11.38*dp4+267.6;
        //double d1=0.92,d2=0.92*10,d3=6.505,d4=11.23;
        //double d1=0.,d2=0.92*11,d3=0.92*7,d4=11.995;
        double x1=0.,y1=0.,x2=11*0.92,y2=0.,y3=7*0.92,x3=0.,x4=11*0.92,y4=7*0.92;
        double[][] arrayA={{2*(x1-x2),2*(y1-y2)},{2*(x1-x3),2*(y1-y3)},{2*(x1-x4),2*(y1-y4)}};
        double[][] arrayB={{(d2*d2-d1*d1+x1*x1-x2*x2+y1*y1-y2*y2)},{(d3*d3-d1*d1+x1*x1-x3*x3+y1*y1-y3*y3)},{(d4*d4-d1*d1+x1*x1-x4*x4+y1*y1-y4*y4)}};
        Matrix A = new Matrix(arrayA);
        Matrix b = new Matrix(arrayB);
        Matrix x = (((A.transpose().times(A)).inverse()).times(A.transpose())).times(b);
        printTest("A:\n");
        for(int i=0;i<3;i++) {
            for(int j=0;j<2;j++) {
                printTest(fixedWidthDoubletoString(A.get(i, j),10,3));
            }
            printTest("\n");
        }
        printTest("b:\n");
        for(int i=0;i<3;i++) {
            printTest(fixedWidthDoubletoString(b.get(i, 0),10,3));
        }
        printTest("\n");
        printTest("x:\n");
        for(int i=0;i<2;i++) {
            printTest(fixedWidthDoubletoString(x.get(i, 0),10,3));
        }



        //调用拟合函数，将信号强度转为距离
      /*  d1=nihe(dp1);
        d2=nihe(dp2);
        d3=nihe(dp3);
        d4=nihe(dp4);

        double x1,x2,x3,x4,y1,y2,y3,y4;
        x1=0;y1=0;x2=0;y2=11;x3=7;y3=11;x4=7;y4=0;
        printTest("Test of Matrix Class.\n\n");
      //  Date start_time = new Date();

        double[][] array = {{0, -22,1}, {-14, -22,2}, {-14, 0,3}};    //最小二乘定位（4个参考点）里的A
      //  double[][] array = {{1.,2.,3.},{4.,5.,6.},{7.,8.,10.}};

        //最小二乘定位（4个参考点）里的b
        double[] array1= {(d2*d2-d1*d1)+(x1*x1-x2*x2)+(y1*y1-y2*y2),(d3*d3-d1*d1)+(x1*x1-x3*x3)+(y1*y1-y3*y3),(d4*d4-d1*d1)+(x1*x1-x4*x4)+(y1*y1-y4*y4)};

        Matrix A = new Matrix(array);
        Matrix b = new Matrix(array1,3);
      //  Matrix b = Matrix.random(3,1);
        Matrix x = A.solve(b);                       //线性运算出x(x,y),得到定位的坐标
      //  Matrix Residual = A.times(x).minus(b);
       // double rnorm = Residual.normInf();
        printTest("A:\n");*/
        /*for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                printTest(fixedWidthDoubletoString(A.get(i, j), 10, 3));
            }
            printTest("\n");
        }
        printTest("b:\n");
        for(int i=0;i<3;i++) {
            printTest(fixedWidthDoubletoString(b.get(i, 0),10,3));
        }
        printTest("\n");

        printTest("x:\n");
        for(int i=0;i<3;i++) {
            printTest(fixedWidthDoubletoString(x.get(i, 0),10,3));
        }
        printTest("\n");*/

    //    printTest("\nrnorm = " + fixedWidthDoubletoString(rnorm,10,3) + " \n");

     //   Date stop_time = new Date();
     //   double etime = (stop_time.getTime() - start_time.getTime())/1000.;
     //   printTest("\nElapsed Time = " + fixedWidthDoubletoString(etime,10,3) + " seconds\n");
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
