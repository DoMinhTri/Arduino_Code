//////////////////////////////////////////////////////////////////////////////////
void PageIndex() 
{
  String tpchan = "" , tpgoc = "" , act = "";
  ///////////////////////
  if (server.hasArg("chan")){ tpchan = server.arg("chan").c_str(); }
  if (server.hasArg("goc")) { tpgoc  = server.arg("goc").c_str();  }
  if (server.hasArg("act")) { act    = server.arg("act").c_str();  }
  ///////////////////////
  if(tpchan != "")
  {
    int goc = tpgoc.toInt();
    //  1B  1A  -  4A 4B
    //  2B  2A  -  3A 3B
    /////////////////
    if(tpchan == "c1a"){ C_1A.write(goc); } //  C_1A
    if(tpchan == "c1b"){ C_1B.write(goc); } //  C_1B
    
    if(tpchan == "c2a"){ C_2A.write(goc); } //  C_2A
    if(tpchan == "c2b"){ C_2B.write(goc); } //  C_2B

    if(tpchan == "c3a"){ C_3A.write(goc); } //  C_3A
    if(tpchan == "c3b"){ C_3B.write(goc); } //  C_3B
    
    if(tpchan == "c4a"){ C_4A.write(goc); } //  C_4A 
    if(tpchan == "c4b"){ C_4B.write(goc); } //  C_4B
  }
  /////////////////////// 
  if(act = "ditoi")   { KieuHoatDong = 1;   } // Nhen_DiToi() ;
  //if(act = "nam")     { Nhen_Nam() ;     }
  //if(act = "dung")    { Nhen_Dung();     }
  //if(act = "ngocdau") { Nhen_NgocDau();  }
  //if(act = "ngocdich"){ Nhen_NgocDich(); }
  ///////////////////////       
  server.send(200, "text/html", "<div align=center> Spider </div>" );
  ///////////////////////
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
