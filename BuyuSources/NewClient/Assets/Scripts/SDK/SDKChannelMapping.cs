using System;
using System.Collections.Generic;

class SDKChannelMapping
{
    Dictionary<uint, string> ChannelMapping = new Dictionary<uint, string>();
    public void Init()
    {
        //const string str = "千尺,qianchi,35i,35i,360,360,17173,37wan,3G门户,3gportal,4399,4399,49游,49you,爱点,aidian,爱点,aidian_,安锋网,anfan,安智,anzhi,应用汇,appchina,百度,baiduyidongyouxi,百度,baidumobilegame,百纳,baina,宝软,baoruan,三星,boyakenuo,糖果游戏,candygame,楚游,chuyou,酷派,coolpad441,酷派,coolpad,酷狗,kugou,当乐,dangle,飞流,feiliu,机锋,gfan,聚乐,htc,华为,huawei,凤凰网,ifeng,电信,igame,金立,jinli,久邦数码,jiubang,酷动,kudong,酷我,kuwo,乐逗,ledou,联想,lenovoopenid,掌上乐游,leyou,定制自有计费,lj_self_sdk,冒泡客,maopao,37玩,37,魅族,meizu,移动,mobile,木蚂蚁,mumayi,拇指玩,muzhiwan,N多市场,nduo,欧朋,opera,欧朋,opera_,OPPO,oppo,泡椒,paojiao,琵琶网,pipawang,PPS,pps,PPTV,pptv,起点,qidian,人人,renren,手盟,shoumeng,手游天下,shouyoutianxia,新浪,sina,斯凯,sky,搜狗,sogou,益玩,yiwan,天天玩,ttw,UC,uc,联通,unicom_,悠悠村,uucun,VIVO,vivo,豌豆荚,wandoujia,万普,wanpu,蜗牛,woniu,小米,xiaomi,迅雷,xunlei,艺果,yiguo,优酷,youku,游龙,youlong,应用宝,yyb,丫丫玩,yyw,暴风,baofeng,暴风,baofeng2,贝壳,beike,冒泡,moposns,掌娱天下,zhangyutianxia,偶玩,youmi,哔哩哔哩,bilibili,乐游天下,leyoutx,卓易,zhuoyi,卓易,zhy,Itools,itools,XY助手,xy,同步推,tongbutui,爱思,aisi,爱苹果,iapple,当乐,dangle4,快用,kuaiyong,快用,ky,PP助手,ppzhushou,91助手,91zhushou,乐8,le8,游戏群,youxiqun,海马玩,haimawan,海马,haima,c1wan,c1wan,YY手游,yygame,布卡漫画,buka,斗鱼,douyu,果盘,guopan,今日头条,jinritoutiao,君海,junhai,靠谱,kaopu,乐视,leshi,美图,meitu,盛大,shengda,搜狐,souhu_new,沃友玩,woyouwan,要玩,yaowan,谊游,yiyou,应用酷,yyk,直播吧,zhibo8,新锐点点,xinrui,虫虫助手,chongchong,夜神,yeshen,玖度,jiudu,猎宝,liebao";
        //string[] strList = str.Split(',');
        //for(int i = 0; i < strList.Length; i += 2)
        //{
        //    byte[] ss = System.Text.UnicodeEncoding.Default.GetBytes(strList[ i + 1]);
        //    uint key = Crc.Crc32(ss, 0, ss.Length);
        //    ChannelMapping.Add(key, strList[i]);
        //}
    }
    public string GetChannelString(string code)
    {
        byte[] ss = System.Text.UnicodeEncoding.Default.GetBytes(code);
        uint key = Crc.Crc32(ss, 0, ss.Length);
        string str;
        if (ChannelMapping.TryGetValue(key, out str))
        {
            return str;
        }
        else
            return "";
    }
}
