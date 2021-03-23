#include <Servo.h> 
#include <IRremote.h>
///////////////////
Servo C_SungLazer, ChanChon, ChanChon_1, ChanChon_2, ChanChon_3, ChanChon_4, ChanChon_5, ChanChon_6, ChanChon_7, ChanChon_8,C_1A,C_1B,C_2A,C_2B,C_3A,C_3B,C_4A,C_4B,C_5A,C_5B,C_6A,C_6B;  
///////////////////
#define IR_BIT_LENGTH 1
#define BIT_1 1000          //Binary 1 threshold (Microseconds)
#define BIT_0 400           //Binary 0 threshold (Microseconds)
#define BIT_START 2000      //Start bit threshold (Microseconds)
#define DEBUG 0             //Serial connection must be started to debug


int runtime_debug = 0;
int output_key = 0;
int power_button = 0;

///////////////////
int led   = 12;
int pinIR = 13;
IRrecv IRTrave(pinIR);
decode_results Trave;
///////////////////
int HanhDong     = 0;  // 1=Hanh dong   ---- 0= Khong lam gi
int TocDo        = 5;  // 
int KieuAuto     = 0;  // (0 = step) ---- (1= auto)
int GocQuay      = 0;  //
int ThoiGianCho  = 5;  //
int KieuHoatDong = 0;  // =1 nhom(1,2,3,4,5,6,7,8) -------  =2 nhom (11,22,33,44) --------------- =3 Nhom(41,42 - 4 tren hoac 4 duoi )
/////////////////////////
int ThuTuChanChon = 0;
int ToaDoChan[9]  = {0,0,0,0,0,0,0,0,0};
///////////////////
void setup() 
{ 
  Serial.begin(9600);
  IRTrave.enableIRIn();
  pinMode(led, OUTPUT); 
  ////////////// 
  C_1A.attach(2); 
  C_2A.attach(3); 
  C_3A.attach(4); 
  C_4A.attach(5); 
  //C_5A.attach(6);
  //C_6A.attach(7); 
 
  C_1B.attach(6); 
  C_2B.attach(7); 
  C_3B.attach(8); 
  C_4B.attach(9); 
  //C_5B.attach(12); 
  //C_6B.attach(13);
 C_SungLazer.attach(10);
} 

//----------------------------------- 
void loop() { 
  
 if (IRTrave.decode(&Trave)) 
 {
    int GT = Trave.value ;
    int MaRemote = GiaiMaRemote(GT);
    CaiDatGiaTriHoatDong(MaRemote);
    ShowGiaTriTest(MaRemote);
    ///////////////////////////////////////
    //Serial.print(results.value, HEX);
    SungLazer(GT);
    IRTrave.resume();
 }

 ////////////
 if(HanhDong == 1)
 {
   ///////////////
   switch (KieuHoatDong) 
   {
      case 11:
//            Nhen_QuaPhai();
            break;  
      case 12:
            //Nhen_DiLui();
            break; 
      case 13:
            //Nhen_QuaTrai();
            break; 
      case 14:
            Nhen_Dung();
            Nhen_DiToi();
            break;  
      //---      
     case 1:
           if(KieuAuto==1)
             ChanQuayTuDong();
           else
             Chan_1ChonQuayTheoDo(GocQuay);
           break;
      case 2:
            Nhen_NgocDau();
            HanhDong = 0;
            break;
      case 3:
            Nhen_Nam();
            HanhDong = 0;
            break;      
      case 4:
            Nhen_NgocDich();
            HanhDong = 0;
            break;
      case 5:
            Nhen_Dung();
            HanhDong = 0;
            break;      
          
   }
   ///////////////
 } 

}
//-----------------------------------
// char* TenNut[] = {"Power","Vol +","Vol -","CH +","Ch -","[1]","[2]","[3]","[4]","[5]","[6]","[7]","[8]","[9]","[+]","[0]","[-/--]","Time","Mute","Right","Down","Left","Up" ,"Null","Main menu","Pic menu"};
//  int MaSo[27]   = {101    ,11     ,22     ,33    ,44    ,1    ,2    ,3    ,4    ,5    ,6    ,7    ,8    ,9    ,111  ,222  ,333     ,444   ,555   ,1111   ,2222  ,3333  ,4444 ,5555  ,666        ,777      };
  
int DayTinHieu[1300] = {-2748,15016,-17219,-26334,-10103,-26432,-32696,-8049,-31771,-6894,26878,29546,-23813,12413,-17096,15564,20388,15613,11552,31913,10836,4432,2979,-7558,28700,-26582,20989,-7415,13634,-2415,3635,-18856,31362,-24975,-19105,-1786,-21439,27389,-24342,18430,-26874,6481,30753,25440,-4032,-10649,30256,-208,3425,-3102,-17609,-1881,3453,1029,960,-22411,12196,15663,9706,-7912,-8866,26420,24823,12114,-26150,-2528,28262,-2619,-11971,2575,23515,12643,16635,21174,27753,-4901,-841,1574,29869,11473,17535,-21394,-12317,27789,-32109,19630,5680,13368,27197,7686,-25474,-4328,-28374,-19140,-25182,12457,-903,9803,14708,-17830,-3417,26388,14192,-25435,-12542,-31611,-32292,-10296,-30031,18220,-30296,30704,9930,-1732,-30817,-23082,11360,21506,-28458,24184,-13596,-14034,31149,25331,5679,2711,23472,-14790,28996,-32227,1518,19337,26167,-14993,16371,-9220,-15814,-4661,9264,-22032,-14792,18281,19605,5971,13960,22034,24205,-6611,18639,27608,18503,11309,19133,-9478,-16696,-29178,11631,-1618,28613,18949,17330,-30211,-4363,19737,164,-29376,451,21148,23312,26283,26928,22263,-16401,-32049,12400,2627,-15637,22341,17888,-7099,-22273,-11307,6643,-6777,2581,31784,22697,12234,-22808,28348,-4426,18500,3966,-20871,-20413,13288,11692,7474,-6268,-9335,-21331,27212,-21293,-32273,12264,22379,-28104,-30237,-10823,15251,-17721,24365,21697,1580,-32274,-9688,11709,-30491,-9790,1507,-10655,29942,1694,-7122,12432,19418,1154,-31857,25290,1431,-16804,-15884,21942,10533,-31206,-6501,-6185,-27035,-31242,-28342,7089,19225,19824,19864,24185,23224,-14535,26879,32266,16076,15479,2057,-5502,15728,-25676,24108,-1342,-30715,24960,23080,10951,-23646,-4977,7237,29331,2397,14090,-28803,20194,26275,1808,-22916,2792,-11367,-21223,23295,16789,10011,3406,-2399,22488,32503,-7152,25144,-18200,-16076,31155,15476,16955,-31206,10189,26479,-9087,32252,-6195,1084,14349,-14456,23585,19901,-14657,-30583,-19233,23924,-11654,-2244,-19363,11266,-30564,28219,-5206,5590,-6713,27747,-8699,-17969,-963,15359,-5583,-6927,31759,15190,-1808,9852,17656,-5172,-17704,28757,10532,27647,-16666,-973,-5964,-15521,18157,17662,5704,6163,-32704,-5560,8352,7923,1382,2165,-13263,-13032,15641,-2750,29568,-4436,18505,17815,31686,-18037,-18242,23264,29316,8095,-13013,30616,-20094,-8132,-4358,-22481,-27064,2399,783,2265,18961,-26892,-8944,-17455,17125,4559,-29472,15075,-12873,-22867,4009,-5368,-29737,-16756,-29532,24755,-15495,16886,-10275,23828,-5402,-30948,-24494,9375,-1045,-31235,32327,17024,-9963,-7638,-7630,-4696,-6905,21098,7149,12869,4047,12594,-14956,9868,-6908,-29452,12258,6359,20886,-12971,8707,31786,-27244,11369,1436,16595,-22678,-21498,11528,-1325,26203,-1067,-320,5506,25647,30132,-15640,27173,25970,-2615,1753,22689,9132,-13605,-29824,-30692,-17306,-10577,-15830,-5602,-21484,-30162,-14565,-9238,13793,7416,-31585,10281,-14104,21791,7508,19767,14785,-9640,28299,5268,10407,-6176,-7797,-9519,-18088,-8418,25034,7198,13287,-915,29325,27956,22311,1584,-7440,23319,-23549,1160,-23183,-22774,-23011,-2579,-19751,8210,-23047,-27839,-32689,31159,21048,-27814,20478,-5930,-19283,-2168,16358,28071,-921,6975,-314,-6197,11700,14629,-28985,2411,-29359,5131,15211,27206,-18004,12712,30647,27604,-11275,17720,25333,-26984,24000,2240,12771,19873,-17956,-4415,25664,-23099,-21432,-15421,19621,-5656,18276,28774,24942,8208,-22203,-18756,-8287,20876,9622,-11718,-29797,-26620,9951,-4796,-2292,14998,-10455,-21669,14009,-30860,6599,12788,-9595,1529,-9441,20765,24447,28715,-29762,23364,-27868,-32614,-31626,-16823,3826,28974,25076,29513,5009,-5858,20444,8409,-22426,24199,15481,-3994,1493,31612,-32220,24444,2402,-19728,-31762,-7697,1690,-16727,-23106,24446,-22690,24386,1029,-20652,15458,-30140,-195,26019,3884,31527,21396,-12887,22880,6752,25154,23580,-20779,-12985,-24137,26202,-273,27520,24895,66,-17859,-12839,-27618,-101,25442,25824,-16060,-4860,-26082,-14866,26895,-4464,20240,-26730,29472,32462,-2052,24996,11126,13863,-14224,28056,-1356,-9911,153,16500,-24988,31787,17296,28759,-31992,-13948,-6039,-30879,-32726,29080,7693,-24748,2687,-3242,-5857,-25423,-21527,2541,6169,-19387,-14433,-32738,-14989,-2365,-9813,-27774,7959,12942,26788,16324,-20451,-15577,11675,13861,18004,-28312,10406,2897,-28041,-7693,14064,26012,18483,-10316,14699,23013,14948,-11523,23356,7697,-20573,15230,7478,-768,3426,1367,3134,27247,14148,25311,20011,-9944,32374,17450,32503,17832,21151,11073,-23520,-1822,23881,-6061,-10530,-25280,-2690,6557,11121,3549,-27107,21118,-27842,595,-541,-15284,14043,-8458,-19056,8259,24181,18184,-2006,-13660,29780,-25995,-11584,8444,18135,16998,-15046,31834,28907,-11809,15246,7252,12695,-8143,-6980,-30482,3356,30416,19370,-13182,-8951,-30027,14022,-29745,14537,24707,2997,-5468,-26280,-23597,-17266,-28645,14544,8921,-2581,-29746,9286,21037,3885,4138,8757,3537,-26808,-4000,24828,-30139,-1833,1675,-18688,-12972,28680,15827,473,-4347,-21579,8730,-19926,18257,-17728,-3921,18389,15406,26181,28071,25928,-10161,11172,-13371,-13933,-19508,-15828,-32308,-20705,-14308,5504,29688,32096,8070,-25450,-24039,17376,-15522,-26965,15655,5423,15660,9274,-24702,-4247,-9500,-25780,-13679,409,-8505,14194,-22189,-2316,29412,-11256,9018,18148,4647,14796,-14393,18643,-24868,-20708,28084,-8440,-6751,28664,-25719,21402,-31831,24952,-7151,17452,26077,10208,-19401,27921,7942,-20616,24795,25652,-5191,-27848,17633,-6312,-9937,9891,32534,-15053,11567,18871,16385,-17772,-7962,-6461,-3183,-13009,-30542,-8749,-32083,-29955,-29086,-23539,5689,-26656,-32752,14613,-31784,-9980,-19885,19785,25234,-21707,-16584,27336,-7336,32232,13970,4538,-20920,-6725,16496,15228,23122,-3732,-3648,-14361,-408,26552,12866,-30738,14527,11389,19092,-28235,-21720,5832,27645,16984,26378,3248,11495,-29001,-27171,-27480,3143,6518,22921,5251,-1984,27808,-18415,-22737,-5915,19533,-26184,54,20078,15168,14951,32048,-1237,-19885,-10903,-18400,7540,28444,971,9790,11192,17923,32121,-23262,22283,28783,14809,-21755,28785,-18702,24592,9565,14269,21553,5276,27065,9990,-5773,922,11416,17863,15142,19450,-22022,-6208,6468,-15506,26738,17488,26842,-26486,-19034,-26398,-1626,2657,-1575,13178,-7079,4767,18156,-13686,10201,-3864,24031,28232,-5268,18032,-12283,1806,-2615,6792,-10508,10050,11071,-1520,28917,-9934,-19853,-3978,10868,-17040,27604,-242,20530,6432,2569,-10407,-1131,5638,30523,-30417,25215,28090,-16767,-14609,21378,-4703,-18632,-10666,25903,32354,28527,-26713,23733,23382,5533,26582,25956,-6301,-657,31354,-29100,-32206,-5708,31039,-5669,32306,-26950,13455,-9672,6011,32335,6942,-2758,-2186,13610,16828,2946,-24960,28018,16717,-15246,-19263,-8364,-12436,-18206,11346,13340,21284,-19262,15405,9275,3683,25139,931,-1678,32098,24949,26006,15743,26795,24748,-6218,-9539,16937,-15154,-16106,-9591,-10214,20023,3314,9467,-19294,32331,-14765,-19036,23965,-2479,-25933,31800,29299,10455,-14978,5354,593,-21278,-9092,-30011,7712,28833,21075,-13939,18390,28478,-4585,21097,-32416,-30674,30427,22090,-11222,-10642,30102,8638,178,-15107,-14423,-11201,26112,15906,-7086,-14885,-8770,-862,-7502,24927,9463,6269,-26812,-7802,9627,27719,11042,-3649,-13362,-10554,-24183,5357,14224,27120,29962,29848,24271,20888,-266,-7126,1162,25084,-6117,17447,-7692,8560,28831,21124,-9349,-11058,21292,26192,-8217,-26073,9856,30492,25968,-15804,21513,30714,-12487,-29962,10456,-24556,-8738,-32083,-14012,27504,11562,-30491,18293,-1450,25563,22113,-12085,28760,-3496,-2102,19732,-21846,5589,-5250,-4834,26164,9661,13537,-26810,24857,-18127,-6940,-4863,31332,27915,7099,-15374,11144,-4767,18981,6788,-30216,12146,29313,-27337,12462,-17466,-11268,18843,-19582,-18082,22510,31592,28350,25139,7408,-6922,31818,1195,-20692,28518,15112,-32437,15042,2304,-27001,9618,-18543,2403,-7666,7543,25935,-16570,-11060,1238,22335,-22317,32273,12206,-19957,-2973,-26093,-31371,7080,-13309,19104,-1306,-14087,-1545,-1617,31171,-8748,12859,-31235,-22846,-20721,-17836,-10191,31878,-25439,10868,-14193,23507,27798,32466,-19537,-20489,7913,-3845,-2191,24757,32233,13015,-10543,10533,21201,8414,14807,13014,-17617,16186,-13727,22406,-26925,27978,-20760,18736,-20715,-1234,17225,-26640,1735,8239,20057,-26157};
int DayChiSo[1300]   = {101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,222,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,333,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,444,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,555,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,1111,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,2222,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,3333,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,4444,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,5555,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,666,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777,777};

int GiaiMaRemote(int TinHieu)
{
  for(int i=0;i<1300;i++)
  {
    if(TinHieu == DayTinHieu[i])
    {
      //NhayDenLed();
      return DayChiSo[i];
      break;
    }
  }
}
//-----------------------------------
void CaiDatGiaTriHoatDong(int MaRIR)
{
   switch (MaRIR) 
   {
    //---------------------------- Chan le hoat dong
     case 1:
           ChanChon      = C_1A;
           HanhDong      = 1;
           KieuHoatDong  = 1;
           ThuTuChanChon = 0;
           GocQuay = ToaDoChan[0];
      break;
    ///////////
    case 2:
          ChanChon      = C_2A; 
          HanhDong      = 1;
          KieuHoatDong  = 1;
          ThuTuChanChon = 1;
          GocQuay = ToaDoChan[1];
      break;
    ///////////  
    case 3:
         ChanChon      = C_3A;
         HanhDong      = 1;
         KieuHoatDong  = 1;
         ThuTuChanChon = 2;
         GocQuay = ToaDoChan[2];
      break;
    ///////////  
    case 4:
         ChanChon      = C_4A;
         HanhDong      = 1;
         KieuHoatDong  = 1;
         ThuTuChanChon = 3;
         GocQuay = ToaDoChan[3];
      break;
    ///////////  
    case 5:
         ChanChon      = C_1B;
         HanhDong      = 1;
         KieuHoatDong  = 1;
         ThuTuChanChon = 4;
         GocQuay = ToaDoChan[4];
      break;
    ///////////  
    case 6:
         ChanChon      = C_2B;
         HanhDong      = 1;
         KieuHoatDong  = 1;
         ThuTuChanChon = 5;
         GocQuay = ToaDoChan[5];
      break;
    ///////////  
    case 7:
         ChanChon      = C_3B;
         HanhDong      = 1;
         KieuHoatDong  = 1;
         ThuTuChanChon = 6;
         GocQuay = ToaDoChan[6];
      break;
    ///////////  
    case 8:
         ChanChon      = C_4B;
         HanhDong      = 1;
         KieuHoatDong  = 1;
         ThuTuChanChon = 7;
         GocQuay = ToaDoChan[7];
      break;  
    //----------------------- Cac chi so
    case 11:
             GocQuay  = GocQuay + TocDo;
             if(GocQuay > 180) GocQuay = 180;
             KieuHoatDong = 1;
             HanhDong = 1;
      break;    
    /////////
    case 22:
             GocQuay  = GocQuay - TocDo;
             if(GocQuay < 0) GocQuay = 0;
             KieuHoatDong = 1;
             HanhDong = 1;
      break;     
    /////////
    case 33:
             ThoiGianCho = ThoiGianCho + 1;
      break;    
    /////////
    case 44:
             ThoiGianCho = ThoiGianCho - 1;
             if(ThoiGianCho<1) ThoiGianCho = 1;
      break;         
    //----------------------- Cac hanh dong
    case 111:
         TocDo = (TocDo + 1) % 101;
         if(TocDo==0) TocDo = 1;
      break;   
    /////////      
    case 222:
         KieuAuto = (KieuAuto + 1)%2;
      break;         
    /////////
    case 333:
         KieuHoatDong = (KieuHoatDong + 1) % 6;
         if(KieuHoatDong==0) KieuHoatDong = 1;
         HanhDong     = 1;
      break;
    //-----  lui - toi - trai - phai - default
    case 1111:
         KieuHoatDong = 11;
         HanhDong     = 1 ;
      break;  
    /////////      
    case 2222:
         KieuHoatDong = 12;
         HanhDong     = 1 ;
      break;
    /////////      
    case 3333:
         KieuHoatDong = 13;
         HanhDong     = 1 ;
      break;
    /////////      
    case 4444:
         KieuHoatDong = 14;
         HanhDong     = 1 ;
      break;      
    /////////// default 
    case 5555:
          HanhDong     = 0; // 1=Hanh dong   ---- 0= Khong lam gi
          TocDo        = 1; // 
          KieuAuto     = 0; // (0 = step) ---- (1= auto)
          GocQuay      = 0; //
          ThoiGianCho  = 5; //
          KieuHoatDong = 0; // =1 nhom(1,2,3,4,5,6,7,8) -------  =2 nhom (11,22,33,44) --------------- =3 Nhom(41,42 - 4 tren hoac 4 duoi )
          Nhen_Nam();
      break;      
     
  }
}
//-----------------------------------
void ChanQuayTuDong()
{
    for(int i=-45;i<=190;i++)
    {
      ChanChon.write(i);
      delay(ThoiGianCho);
    }
    //////////
    for(int i=190;i>=-45;i--)
    {
      ChanChon.write(i);
      delay(ThoiGianCho);
    }
}
//-----------------------------------
//-----------------------------------
void Chan_1ChonQuayTheoDo(int GocChonQuay) { Quay1Chan(ChanChon, ThuTuChanChon, GocChonQuay); };
//-----------------------------------
void SungLazer(int GTN)
{
  int DoQuay = random(30,180);//map(GTN, 0, 1023, 30, 180);
  Quay1Chan(C_SungLazer, 8, DoQuay);
}
//-----------------------------------
void Nhen_DiToi()
{
  //Quay4Chan_Dui(50,100,100,130);
 // Quay4Chan_Ban(180,180,0,0);
    ThoiGianCho = 1; 
    Quay2Chan_15(40,150 );
    QuayChan_5(180);
    Quay2Chan_37( 40,30);
    QuayChan_7(   0);
    Quay2Chan_68(150,30 );
    Quay2Chan_13( 110,110);
    Quay2Chan_48(150,0 );
    Quay2Chan_26( 150,180);
    Quay2Chan_57( 150,30);
    Quay2Chan_24( 80,80);
}
//-----------------------------------
void Nhen_Dung()
{
  Quay4Chan_Dui(50,100,100,130);
  Quay4Chan_Ban(180,180,0,0);
}
//-----------------------------------
void Nhen_Nam()
{
  Quay4Chan_Dui(100,100,100,100);
  Quay4Chan_Ban(45,70,110,115);
}
//-----------------------------------
void Nhen_NgocDau()
{
  Quay4Chan_Dui(10,100,100,170);
  Quay4Chan_Ban(180,60,120,0);
}
//-----------------------------------
void Nhen_NgocDich()
{
  Quay4Chan_Dui(25,100,100,170);
  Quay4Chan_Ban(60,180,0,140);
}
//-----------------------------------
//-----------------------------------
void QuayChan_1(int TDo){ Quay1Chan(C_1A, 0, TDo) ;}
void QuayChan_2(int TDo){ Quay1Chan(C_2A, 1, TDo) ;}
void QuayChan_3(int TDo){ Quay1Chan(C_3A, 2, TDo) ;}
void QuayChan_4(int TDo){ Quay1Chan(C_4A, 3, TDo) ;}
void QuayChan_5(int TDo){ Quay1Chan(C_1B, 4, TDo) ;}
void QuayChan_6(int TDo){ Quay1Chan(C_2B, 5, TDo) ;}
void QuayChan_7(int TDo){ Quay1Chan(C_3B, 6, TDo) ;}
void QuayChan_8(int TDo){ Quay1Chan(C_4B, 7, TDo) ;}
//------
void Quay1Chan(Servo &Chan, int ThuTu, int ToaDo)
{
  int TDTam = ToaDoChan[ThuTu]; ToaDoChan[ThuTu] = ToaDo;
  while(TDTam != ToaDo)
  {
    if(TDTam > ToaDo) TDTam--;
    if(TDTam < ToaDo) TDTam++;
    Chan.write(TDTam);
    delay(ThoiGianCho);
  }
}
//------

void Quay2Chan_13(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_1A, 0, ToaDo_1, C_3A, 2, ToaDo_2); }
void Quay2Chan_15(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_1A, 0, ToaDo_1, C_1B, 4, ToaDo_2); }

void Quay2Chan_24(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_2A, 1, ToaDo_1, C_4A, 3, ToaDo_2); }
void Quay2Chan_26(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_2A, 1, ToaDo_1, C_2B, 5, ToaDo_2); }

void Quay2Chan_37(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_3A, 2, ToaDo_1, C_3B, 6, ToaDo_2); }
void Quay2Chan_46(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_4A, 3, ToaDo_1, C_2B, 5, ToaDo_2); }
void Quay2Chan_48(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_4A, 3, ToaDo_1, C_4B, 7, ToaDo_2); }
void Quay2Chan_57(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_1B, 4, ToaDo_1, C_3B, 6, ToaDo_2); }
void Quay2Chan_68(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_2B, 5, ToaDo_1, C_4B, 7, ToaDo_2); }
void Quay2Chan_56(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_1B, 4, ToaDo_1, C_2B, 5, ToaDo_2); }
void Quay2Chan_78(int ToaDo_1, int ToaDo_2){ Quay2Chan(C_3B, 6, ToaDo_1, C_4B, 7, ToaDo_2); }


void Quay2Chan(Servo &_Chan_1, int TTu_1, int ToaDo_1, Servo &_Chan_2, int TTu_2, int ToaDo_2)
{
    int TDo_1 = ToaDoChan[TTu_1]; ToaDoChan[TTu_1] = ToaDo_1;
    int TDo_2 = ToaDoChan[TTu_2]; ToaDoChan[TTu_2] = ToaDo_2;
    
    int Dem_1 =1, Dem_2 =1;
    int QuayChanHoanTat = 0;
    
    if(TDo_1 > ToaDo_1) Dem_1 = -1; if(TDo_1 == ToaDo_1) { Dem_1 = 0; QuayChanHoanTat++; }
    if(TDo_2 > ToaDo_2) Dem_2 = -1; if(TDo_2 == ToaDo_2) { Dem_2 = 0; QuayChanHoanTat++; }
    while(QuayChanHoanTat !=2)
    {
      //----------
      if(Dem_1 != 0) 
      {
        TDo_1 =  TDo_1 + Dem_1 ;
        if(TDo_1 == ToaDo_1) { Dem_1 = 0; QuayChanHoanTat++; } 
      }
      //----------
      if(Dem_2 != 0) 
      {
        TDo_2 =  TDo_2 + Dem_2 ;
        if(TDo_2 == ToaDo_2) { Dem_2 = 0; QuayChanHoanTat++; } 
      }
      //----------      
      _Chan_1.write(TDo_1);
      _Chan_2.write(TDo_2);
      delay(ThoiGianCho);
    }
}
//------
void Quay4Chan_Chan(int ToaDo_1, int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_2A, 1, ToaDo_1, C_4A, 3, ToaDo_2, C_2B, 5, ToaDo_3, C_4B, 7, ToaDo_4); }
void Quay4Chan_Le(int ToaDo_1  , int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_1A, 0, ToaDo_1, C_3A, 2, ToaDo_2, C_1B, 4, ToaDo_3, C_3B, 6, ToaDo_4); }
void Quay4Chan_Dui(int ToaDo_1 , int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_1A, 0, ToaDo_1, C_2A, 1, ToaDo_2, C_3A, 2, ToaDo_3, C_4A, 3, ToaDo_4); }
void Quay4Chan_Ban(int ToaDo_1 , int ToaDo_2, int ToaDo_3, int ToaDo_4) {  VeToaDoChon_4(C_1B, 4, ToaDo_1, C_2B, 5, ToaDo_2, C_3B, 6, ToaDo_3, C_4B, 7, ToaDo_4); }
//------
void VeToaDoChon_4(Servo &_Chan_1, int TTu_1, int ToaDo_1, Servo &_Chan_2, int TTu_2, int ToaDo_2, Servo &_Chan_3, int TTu_3, int ToaDo_3, Servo &_Chan_4, int TTu_4, int ToaDo_4)
{
    int TDo_1 = ToaDoChan[TTu_1]; ToaDoChan[TTu_1] = ToaDo_1;
    int TDo_2 = ToaDoChan[TTu_2]; ToaDoChan[TTu_2] = ToaDo_2;
    int TDo_3 = ToaDoChan[TTu_3]; ToaDoChan[TTu_3] = ToaDo_3;
    int TDo_4 = ToaDoChan[TTu_4]; ToaDoChan[TTu_4] = ToaDo_4;
    
    int Dem_1 =1, Dem_2 =1, Dem_3 =1, Dem_4 =1;
    int QuayChanHoanTat = 0;
    
    if(TDo_1 > ToaDo_1) Dem_1 = -1; if(TDo_1 == ToaDo_1) { Dem_1 = 0; QuayChanHoanTat++; }
    if(TDo_2 > ToaDo_2) Dem_2 = -1; if(TDo_2 == ToaDo_2) { Dem_2 = 0; QuayChanHoanTat++; }
    if(TDo_3 > ToaDo_3) Dem_3 = -1; if(TDo_3 == ToaDo_3) { Dem_3 = 0; QuayChanHoanTat++; }
    if(TDo_4 > ToaDo_4) Dem_4 = -1; if(TDo_4 == ToaDo_4) { Dem_4 = 0; QuayChanHoanTat++; }
    
    while(QuayChanHoanTat !=4)
    {
      //----------
      if(Dem_1 != 0) 
      {
        TDo_1 =  TDo_1 + Dem_1 ;
        if(TDo_1 == ToaDo_1) { Dem_1 = 0; QuayChanHoanTat++; } 
      }
      //----------
      if(Dem_2 != 0) 
      {
        TDo_2 =  TDo_2 + Dem_2 ;
        if(TDo_2 == ToaDo_2) { Dem_2 = 0; QuayChanHoanTat++; } 
      }
      //----------
      if(Dem_3 != 0) 
      {
        TDo_3 =  TDo_3 + Dem_3 ;
        if(TDo_3 == ToaDo_3) { Dem_3 = 0; QuayChanHoanTat++; } 
      }      
      //----------
      if(Dem_4 != 0) 
      {
        TDo_4 =  TDo_4 + Dem_4 ;
        if(TDo_4 == ToaDo_4) { Dem_4 = 0; QuayChanHoanTat++; } 
      }  
      //----------      
      _Chan_1.write(TDo_1);
      _Chan_2.write(TDo_2);
      _Chan_3.write(TDo_3);
      _Chan_4.write(TDo_4);
      delay(ThoiGianCho);
    }
}

//-----------------------------------
//-----------------------------------
void NhayDenLed()
{
   digitalWrite(led, HIGH);  // turn the LED on (HIGH is the voltage level)
   delay(100);               // wait for a second
   digitalWrite(led, LOW);   // turn the LED off by making the voltage LOW
   delay(100);  
}
//-----------------------------------
void ShowGiaTriTest(int MaIR)
{
  char* _TrangThai[]   ={"Off","On"};
  char* _MangAuto[]    ={"Chinh tay","Tu dong"};
  char* _MangHoatDong[]={"No thing","nhom(1-2-3-4-5-6-7-8)","Ngoc dau","Nam","Doc dich","Dung","","","","","","Qua phai","Di lui","Quan trai","Di toi"};
  Serial.println("--------------------------------------------------------------------");
  Serial.print("Ma IR: ")         ;Serial.println(MaIR);
  Serial.print("Hanh dong : ")    ;Serial.println(_TrangThai[HanhDong]);  
  Serial.print("Toc do: ")        ;Serial.println(TocDo);
  Serial.print("Auto: ")          ;Serial.println(_TrangThai[KieuAuto]);
  Serial.print("GocQuay: ")       ;Serial.println(GocQuay);
  Serial.print("Thoi gian cho: ") ;Serial.println(ThoiGianCho);
  Serial.print("Nhom chon : ")    ;Serial.println(_MangHoatDong[KieuHoatDong]);
}

//-----------------------------------
//////////////////////////
void read_pulse(int data[], int num_bits)
{
  for (int i = 0; i < num_bits; i++)
  {
    data[i] = pulseIn(pinIR, LOW);
  }
}
//////////////////////////
void pulse_to_bits(int pulse[], int bits[], int num_bits)
{
  //Serial.println(num_bits);  
  if (DEBUG || runtime_debug) { Serial.println("-----"); }

  for(int i = 0; i < num_bits ; i++)
  {
    if (DEBUG || runtime_debug) { Serial.println(pulse[i]); }

    if(pulse[i] > BIT_1) //is it a 1?
    {
      bits[i] = 1;
    }
    else if(pulse[i] > BIT_0) //is it a 0?
    {
      bits[i] = 0;
    } 

    else //data is invalid...
    {
      //Serial.println("Error");
    }
  }
}
//////////////////////////
int bits_to_int(int bits[], int num_bits)
{
  int result = 0;
  int seed = 1;

  //Convert bits to integer
  for(int i = 0 ; i < num_bits ; i++)
  {
    if(bits[i] == 1)
    {
	result += seed;
    }

    seed *= 2;
  }

  return result;
}
//////////////////////////
int get_ir_key()
{
  int pulse[IR_BIT_LENGTH];
  int bits[IR_BIT_LENGTH];  

  do {} //Wait for a start bit
  while(pulseIn(pinIR, LOW) < BIT_START);

  read_pulse(pulse, IR_BIT_LENGTH);

  pulse_to_bits(pulse, bits, IR_BIT_LENGTH);

  return bits_to_int(bits, IR_BIT_LENGTH);
}


