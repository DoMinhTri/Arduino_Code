//////////////////////////////////////////////////////////////////////////////////
void PageIndex() 
{
  String tpchan = "" , tpgoc = "" , act = "";
  ///////////////////////
  if (server.hasArg("chan")){ tpchan = server.arg("chan").c_str(); }
  if (server.hasArg("goc")) { tpgoc  = server.arg("goc").c_str();  }
  if (server.hasArg("act")) { act    = server.arg("act").c_str();  KieuHoatDong = act.toInt();  Serial.println(KieuHoatDong); }
  ///////////////////////
  if(tpchan != "")
  {
    int goc = tpgoc.toInt();
    //  1B  1A  -  4A 4B
    //  2B  2A  -  3A 3B
    /////////////////
    if(tpchan == "sv1"){ myServo1.write(goc); } //  C_1A
    if(tpchan == "sv2"){ myServo2.write(goc); } //  C_1B
    
    if(tpchan == "sv3"){ myServo3.write(goc); } //  C_2A
    if(tpchan == "sv4"){ myServo4.write(goc); } //  C_2B

    if(tpchan == "sv5"){ myServo5.write(goc); } //  C_3A
    if(tpchan == "sv6"){ myServo6.write(goc); } //  C_3B
    
    if(tpchan == "sv7"){ myServo7.write(goc); } //  C_4A 
    if(tpchan == "sv8"){ myServo8.write(goc); } //  C_4B
  }
  /////////////////////// 
  //if(act = "ditoi")     { KieuHoatDong = 1; } // Nhen_DiToi() ;
  //if(act = "nam")     { Nhen_Nam() ;      }
  //if(act = "dung")    { Nhen_Dung();      }
  //if(act = "ngocdau") { Nhen_NgocDau();   }
  //if(act = "ngocdich"){ Nhen_NgocDich();  }
  //server.send(200, "text/html",  KieuHoatDong  );
  ///////////////////////       
  server.send(200, "text/html", "<div align=center> Spider action: " + act + " </div>" );
  ///////////////////////
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
