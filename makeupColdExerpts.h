/* 今附MIT.license分享 粉黛·霜寒 的内容节选, 献自 @大冰块stupid吗 */

/* tan(0.5πx):
数值近似，在计算机GPU上有可能倒扣性能。
之所以带系数0.5π，
    是为在流星的渲染中节约一次乘法，
    你可以修改近似多项式的常系数来免去0.5π。
近似算法
    涉及tan(x)=1/tan(0.5π-x)，
    它假定除法是相对廉价的运算。 */
float tan05pi(float x){
    bool far0=abs(x)>0.5;
    if(far0) x=-x+sign(x); // 三角恒等式
    x*=2.0;
    float x3=x*x*x;
    x=0.2388*x3+(1.0-0.2388)*x;
    if(far0) x=1.0/x; // 三角恒等式
    return x; // @大冰块stupid吗
}

/* arcsin(x)/(2π):
也是数值近似，并以类似的理由带系数1/(2π)。
近似算法
    涉及arcsin(x)=arccos(sqrt(1-x^2))，
    实际使用*inversesqrt替代sqrt。
多项式系数的搭配
    使得近似函数与原函数在x=sqrt(0.5)处严格相切，
    这意味着它的数值误差还可以更小，
    这样设计是为令流星的运动轨迹顺滑，并容忍数值误差。 */
float arcsin_2pi(float x){
    bool minus=x<0.0;
    bool far0=abs(x)>0.7071067812;
    if(far0){
        x=1.0-x*x; // 正定
        x*=inversesqrt(x); // 三角恒等式
    }float x3=x*x*x;
    x=(0.1366197724 *0.3535533906) *x3
    +((1.0 -0.1366197724) *0.1767766953) *x;
    if(far0){
        x=0.25-x; // 三角恒等式
        if(minus) x=-x;
    }return x; // @大冰块stupid吗
}

/* 余下内容都是圆角方块层叠云套装，由于内容繁复，这里只提它的特色，实现思路不难，AI能够轻易解读。
套装用整数表存储一个16x16的所谓云区块，
    这使得云朵16x16地重复，
    值得注意的是，由于人类很难方便地认出不同视角下的同一个几何体，不必担心玩家看穿重复，
    这很可能比预烘焙贴图快得多，
    这个整数表是用位运算表示的，然而在高版本glsl，你完全可以使用形如0b101的二进制字面量，
    二进制呈现整数表，是为了方便地在编辑器中直观修改云朵样式，
※ 特别注意，在发布版中，你必须选中所有的 0_ 并将它们改为 0 _，即插入空格，对1_做相同的处理，
    因为尽管绝大多数编译器，都先将0解析为整数后，剩下_，然后将_宏展开，
    但是在极个别移动设备搭载img gpu的情况下，0_被视为非法字面量，被拒绝编译，
    这里不插入空格是为了让整数表看起来更像是正方形。 */
#define lefBracket32 (((( (((( (((( (((( (((( (((( (((( ((((
#define rytBracket31 )))) )))) )))) )))) )))) )))) )))) )))
#define _ )<<1
#define MACRO_cldChunk (uint[8](\
    uint(lefBracket32 0 |0_|0_|0_|0_ |0_|0_|0_|0_ |0_|0_|0_|0_ |0_|0_|0_|0_\
                        |0_|0_|1_|1_ |0_|0_|0_|1_ |1_|0_|0_|0_ |0_|0_|0_|0_),\
    uint(lefBracket32 0 |0_|0_|1_|1_ |0_|0_|0_|0_ |1_|1_|0_|0_ |0_|0_|1_|0_\
                        |0_|0_|0_|0_ |0_|0_|0_|0_ |1_|1_|0_|0_ |0_|0_|0_|0_),\
    uint(lefBracket32 0 |0_|0_|0_|0_ |0_|0_|0_|0_ |0_|0_|0_|0_ |1_|0_|0_|0_\
                        |0_|0_|0_|0_ |0_|1_|0_|0_ |0_|0_|0_|0_ |1_|1_|0_|0_),\
    uint(lefBracket32 0 |0_|1_|1_|0_ |0_|1_|1_|0_ |0_|0_|0_|1_ |0_|0_|0_|0_\
                        |0_|1_|0_|0_ |0_|0_|0_|0_ |0_|0_|0_|0_ |0_|0_|0_|0_),\
    uint(lefBracket32 0 |0_|0_|1_|0_ |0_|0_|0_|0_ |0_|1_|0_|0_ |0_|0_|1_|0_\
                        |0_|0_|0_|0_ |0_|0_|0_|0_ |0_|0_|1_|0_ |0_|1_|1_|0_),\
    uint(lefBracket32 0 |0_|0_|0_|1_ |1_|0_|0_|0_ |1_|1_|1_|0_ |0_|1_|0_|0_\
                        |0_|0_|1_|1_ |0_|1_|0_|0_ |0_|1_|1_|0_ |0_|0_|0_|0_),\
    uint(lefBracket32 0 |0_|0_|0_|1_ |1_|0_|0_|0_ |0_|0_|0_|0_ |0_|0_|0_|0_\
                        |0_|0_|0_|0_ |0_|0_|0_|0_ |0_|0_|0_|0_ |1_|1_|0_|0_),\
    uint(lefBracket32 0 |0_|1_|0_|0_ |0_|0_|0_|1_ |0_|0_|0_|0_ |1_|1_|1_|0_\
                        |0_|0_|0_|0_ |0_|0_|0_|0_ |0_|0_|0_|0_ |0_|1_|0_|0_)\
))
struct bobo{bool x, y;};
bobo hashBobo_round(uint x, uint y, bobo pozOffset){
    bobo res=bobo(false, false);
    uint z=x+(pozOffset.x?1u:15u); z&=15u;
    uint w=y+(pozOffset.y?1u:15u); w&=15u;
    const highp uint cldChunk[8]=MACRO_cldChunk;
    highp uint local=cldChunk[y>>1u]; local>>=(((y&1u)>0u)?0u:16u);
    highp uint neibr=cldChunk[w>>1u]; neibr>>=(((w&1u)>0u)?0u:16u);
    res.x=(((local>>x)&1u)>0u);
    res.y=(((local>>z)&1u)>0u);
    res.y=res.y||(((neibr>>x)&1u)>0u);
    return res;
}
#undef _
#define f2u(f) uint(int(f)) // 这是为了抵抗某些匪夷所思的编译未定义行为
#define MIXTO(from, to, by) from=mix(from, to, by) // 高危语法糖，使用务必加分号
float roundCld2D(vec2 xy, float alpha){
    vec2 fg=floor(xy); // 这是因为int()向0取整
    uint x=f2u(fg.x); uint y=f2u(fg.y);
    x&=15u; y&=15u; fg=fract(xy);
    bobo isCldnotCorner=hashBobo_round(x,y,bobo(fg.x>0.5,fg.y>0.5));
    if(!(isCldnotCorner.x)) return 0.0;
    if(isCldnotCorner.y) return alpha;
    fg=abs(fg-vec2(0.5));
    fg*=2.0; // 归一化
    float rr=dot(fg, fg);
    rr=min(1.0, 2.0-rr);
  #ifndef chunkReady
    return alpha*rr*rr*rr;
  #else
    return alpha*rr*rr;
  #endif
}
vec3 roundCloudIncluded(
    vec3 viewPoz, vec3 viewDir, vec3 sunDir, float absDD, highp float time,
    vec3 cldColor, float cosTheta, float dayClear, vec3 skyColor
){
    sunDir=sunDir.y>0.0 ? sunDir : -sunDir;
    float temp=5.5/viewPoz.y;
    viewPoz.xz*=temp; viewPoz.y=5.5;
    highp vec2 mov=vec2(0.125 *time);
    highp vec2 xy=viewPoz.xz;
  #ifndef chunkReady
    float alpha=0.17*useDC(dayClear, 0.8); // 阴天变淡
  #else // 缺少一半的层，补偿到1-0.83^2
    float alpha=0.31*useDC(dayClear, 0.8); // 阴天变淡
  #endif
    alpha*=clamp(4.0 *(cosTheta-0.15), 0.0, 1.0);
    temp=roundCld2D(xy-mov, alpha);
    cldColor*=1.15;
    if(temp>0.0) MIXTO(skyColor, cldColor, temp);
    float holder=dot(viewDir, sunDir);
    holder=0.987-holder*absDD*0.007;
  #ifndef chunkReady // sky
    holder=0.5*(holder+holder*holder);
    for(lowp int i=0; i<12; ++i){
        xy*=0.991;
        temp=roundCld2D(xy-mov, alpha);
        if(temp>0.0){
            cldColor*=holder;
            MIXTO(skyColor, cldColor, temp);
        }
    }return skyColor;
  #else // renderchunk
    holder*=holder;
    for(lowp int i=0; i<6; ++i){
        xy*=0.982;// 缺少一半的层
        temp=roundCld2D(xy-mov, alpha);
        if(temp>0.0){
            cldColor*=holder;
            MIXTO(skyColor, cldColor, temp);
        }
    }return skyColor;
  #endif
}
