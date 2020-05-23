RWTexture2D<unorm float4> g_Input32 : register(u0);
RWTexture2D<float4> g_Input128 : register(u1);

RWTexture2D<float4> g_Output : register(u2);

int inside(uint x,uint y)
{
    if (100<=x && x <= 200 && 100<=y&& y <= 200)
        return 1;
    return 0;
}
// һ���߳����е��߳���Ŀ���߳̿���1άչ����Ҳ����
// 2ά��3ά�Ų�
[numthreads(16, 16, 1)]
void CS(uint3 DTid : SV_DispatchThreadID,uint3 Gid:SV_GroupID,uint GTid:SV_GroupIndex)
{
    //g_Output[DTid.xy] =  g_Input128[DTid.xy] - g_Input32[DTid.xy] ;
    g_Output[DTid.xy] = float4(inside(DTid.x,DTid.y),0.0, 0.0, 1.0);
}
