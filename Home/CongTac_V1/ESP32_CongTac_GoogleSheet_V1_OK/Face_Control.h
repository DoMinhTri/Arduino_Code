//////////////////////////////////////////////////////////////////////////////////
void PageIndex() 
{
  if (server.hasArg("cw4"))
  {    
        String cw4 = server.arg("cw4").c_str(); 
        ///////////////////
        if(cw4 != "")
        { 
            if(cw4 == "10"){ MangTTDen[0] = 0; UpStas  = true; }
            if(cw4 == "11"){ MangTTDen[0] = 1; UpStas  = true; }
    
            if(cw4 == "20"){ MangTTDen[1] = 0; UpStas  = true; }
            if(cw4 == "21"){ MangTTDen[1] = 1; UpStas  = true; }

            if(cw4 == "30"){ MangTTDen[2] = 0; UpStas  = true; }
            if(cw4 == "31"){ MangTTDen[2] = 1; UpStas  = true; }

            if(cw4 == "40"){ MangTTDen[3] = 0; UpStas  = true; }
            if(cw4 == "41"){ MangTTDen[3] = 1; UpStas  = true; }
            //////////
            if(cw4 == "51"){ delay(1000); Restart(); }
            //////////
            BatDen();
        }
        ///////////////////
  }
  //////////////////////
  if (server.hasArg("wname"))
  {    
    String wname = server.arg("wname").c_str(); 
    if(wname != ""){ Wifi_LuuName(wname); } 
    delay(2000);
    Restart();
  }
  //////////////////////
  ///if(WiFi.status() == WL_CONNECTED) { OnlineIP = LayChuoiIP(WiFi.localIP()); }
  server.send(200, "text/html", TTCongTac() );
}
//////////////////////////////////////////////////////////////////////////////////
void PageSetup() 
{
  String tpData = "";
  if (server.hasArg("sw4_wname")){    
    tpData = server.arg("sw4_wname").c_str(); 
    if(tpData != ""){ Wifi_LuuName(tpData); } 
  }
  ///////////////////////
  tpData = "";
  if (server.hasArg("sw4_wpass")){    
    tpData = server.arg("sw4_wpass").c_str(); 
    if(tpData != ""){ Wifi_LuuPass(tpData); } 
  }
  ///////////////////////
  tpData = "<!DOCTYPE html><html><head><title>UTS_CW4</title></head><body bgcolor='#FAFAFA'><script>function SendData(){var name = document.getElementById('Name').value; var pass = document.getElementById('Pass').value; var url = 'http://192.168.4.1/setup?sw4_wname=' + name + '&sw4_wpass=' + pass + '&sw4_rs=ok'; var xhttp = new XMLHttpRequest(); xhttp.open('GET', url, true); xhttp.send(); document.getElementById('textsms').innerHTML='Complete !';} function getName(_Name){ document.getElementById('Name').value = _Name; }</script><style>body{margin:20px;} .style{width:100%;height:35px;} .bgcolor{background:#00ADEF;} .radio{width:25px;height:25px;} </style><table width='100%' cellspacing='1' cellpadding='1'><tr><td><input type='text' id='Name' class='style' placeholder='Wifi Name'></td></tr><tr><td><input type='text' id='Pass' class='style' placeholder='Wifi Pass'></td></tr><tr><td><table width='100%'><tr><td width='70%' align='center'><font size=3 color=red><b id='textsms'>&nbsp;</b></font></td><td align='right' width='30%'><input type='button' class='style bgcolor' value='Connect' onclick='SendData()'></td></tr></table></td></tr></table>" + ListSSID + "</body></html>";
  server.send(200, "text/html", tpData );
  ///////////////////////
  tpData = "";
  if (server.hasArg("sw4_rs")){    
    tpData = server.arg("sw4_rs").c_str(); 
    if(tpData == "ok"){ delay(2000); Restart(); }
  }
  ///////////////////////
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
