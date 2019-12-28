//
// Created by 叶亮 on 2019/2/14.
//

#include "../XData.h"
#include "IResample.h"
#include "../Xlog.h"

void IResample::Update(XData data)
{
    XData d = this->Resample(data);
    if(d.size>0){
        this->Notify(d);
    }
}