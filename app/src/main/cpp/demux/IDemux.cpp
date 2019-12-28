//
// Created by 叶亮 on 2019/1/27.
//

#include "IDemux.h"
#include "../XData.h"

void IDemux::Main()
{
    while (!isExit)
    {
        if(IsPause())
        {
            XSleep(2);
            continue;
        }

        XData d = Read();
        if(d.size > 0){
            Notify(d);
        }else{
            XSleep(2);
        }
    }
}