# 系統晶片設計實習
電子碩一甲 F112112117 李聖鈞  

## HW3：
1.完成button硬體中斷，button按壓之後的訊號經過FPGA，再中斷CPU  
2.ISR內容: 閃爍LED或者其他  
3.main程式內容: 無窮迴圈計數LED顯示  
4.報告呈現: (1) ISR 與主程式, (2) IRQ編號, (3)中斷相關暫存器名稱、位址、手冊名稱與頁碼  
5中斷相關暫存器：IRQ 號碼, pending, mask/enable, .....  

影片網址：https://youtu.be/oJIv8i3wATM  
透過SW啟動中斷，中斷啟動時停在當前LED燈號2s，兩秒後LED燈號歸0，結束中斷  
