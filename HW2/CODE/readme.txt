系統晶片設計實習
電子碩一甲 F112112117 李聖鈞

HW2：按鈕控制LED
影片網址：https://youtu.be/Oa348FtzoGQ
      SW_num = XGpio_DiscreteRead(&SW_XGpio, 1);//讀取按鈕數值
    	if (SW_num <= 9){ //按鈕數值<=9
    		LED_num = LED_num +1;//LED為正數
    		XGpio_DiscreteWrite(&LED_XGpio, 1, LED_num);//將LED數值寫入GPIO
    	}
    	else{
    		LED_num = LED_num -1;//按鈕數值>9則LED倒數
    		XGpio_DiscreteWrite(&LED_XGpio, 1, LED_num);//將LED數值寫入GPIO
    	}
