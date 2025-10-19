import time, image,sensor,math,pyb,ustruct
from image import SEARCH_EX, SEARCH_DS
from pyb import Pin, Timer,LED
import os


# part1
#----------------------------------- 基本配置 ------------------------------------------------
sensor.reset()  # 重置摄像头到初始配置
sensor.set_contrast(1)   # 设置对比度
sensor.set_gainceiling(16)  # 设置自动增益
sensor.set_framesize(sensor.QQVGA) # 160 x 120 ，减小计算量
sensor.set_pixformat(sensor.GRAYSCALE)  # 模板匹配要求灰度图
sensor.set_windowing((0, 50, 160, 50))  # 直接设置感兴趣区域，减小计算量
sensor.set_auto_gain(False)  # 关闭自动增益
sensor.set_auto_whitebal(False)  # 关闭白平衡

Exposure_scale = 1 # 曝光调节尺度
print(f"< 最初曝光 > {sensor.get_exposure_us()}")
sensor.skip_frames(time = 1000)
current_exposure=sensor.get_exposure_us()
print(f"< 当前曝光 > {current_exposure}")
sensor.set_auto_exposure(False,exposure_us=int(current_exposure* Exposure_scale))

uart = pyb.UART(3, 115200, timeout_char = 1000) # 初始化串口

#----------------------------------- 基本配置 ------------------------------------------------


# part2
#----------------------------------- 参数初始化及读取模板 ------------------------------------------------

Find_Task = 1
Target_Num = 3
find_flag = 0
x = 0
data = [0x00]*8
LoR=0
dir=r"/Template"

# 图片读入
template_base,template_L,template_R,template_LL,template_RR=[],[],[],[],[]
for img_path in sorted(os.listdir(dir),reverse=False):
    value=img_path.split(".")[0]
    value=value if len(value)==1 else value[1:]
    img=image.Image(dir+"/"+img_path)
    index=-1
    if value in ["L","R","LL","RR"]:
        index=["L","R","LL","RR"].index(value)
    else:
        index=-1
    [template_L,template_R,template_LL,template_RR,template_base][index].append(img)

#----------------------------------- 参数初始化及读取模板 ------------------------------------------------
# 检验代码
# for tem in [template_L,template_R,template_LL,template_RR,template_base]:
#     print(f"{len(tem)}")


# part3
#----------------------------------- 功能函数定义 ------------------------------------------------

def UartReceiveDate():
    '''
    UART接收函数,同时对收到的数据进行解码，返回任务和目标数字
    '''
    global Find_Task
    global Target_Num
    global x
    global data

    for i in range(8):data[i]= uart.readchar()

    if  x < 5 and data[x] == 42 and data[x+3] == 38:
        Find_Task =  data[x+1]
        Target_Num = data[x+2]
        Find_Task =  Find_Task - 48
        Target_Num = Target_Num - 48
        x = 0
    elif x >= 5: x = 0
    x+=1
    # print(f"\n< 任务码 > {Find_Task}")
    # print(f"< 目标数字 > {Target_Num}\n")

def FirstFindTemplate(template):
    '''
    寻找目标数字,并返回数字边框
    '''
    return img.find_template(template, 0.8, step=1, roi=(50, 0, 60, 50), search=SEARCH_EX)

def FirstFindedNum(R, Finded_Num):
    '''
    画出数字矩形框,并把【目标数字，方向控制，识别标志，当前任务】发送给主控
    '''
    global Find_Task
    global find_flag
    img.draw_rectangle(R, color=(225, 0, 0))
    find_flag = 1
    Num = Finded_Num
    FH = bytearray([0x2C,0x12,Num, LoR, find_flag, Find_Task,0x5B])
    uart.write(FH)
    print("目标病房号：", Num)

def FindTemplateL(template):
    '''
    左侧模板识别，并返回数字框
    '''
    return img.find_template(template, 0.5, step=1, roi=(0, 0, 70, 50), search=SEARCH_EX)
def FindTemplateR(template):
    '''
    右侧模板识别，并返回数字框
    '''
    R = img.find_template(template, 0.77, step=1, roi=(90, 0, 70, 50), search=SEARCH_EX)
    return R
def FindTemplateLL(template):
    '''
    左左侧模板识别，并返回数字框
    '''
    R = img.find_template(template, 0.79, step=1, roi=(0, 0, 70, 50), search=SEARCH_EX)
    return R
def FindTemplateRR(template):
    '''
    右右侧模板识别，并返回数字框
    '''
    R = img.find_template(template, 0.79, step=1, roi=(90, 0, 70, 50), search=SEARCH_EX)
    return R
def FindedNum(R, Finded_Num):
   global Find_Task
   global find_flag
   '''
   调整小车方向
   '''
   print("temp4")
   img.draw_rectangle(R, color=(225, 0, 0))
   if R[0] >90:
       LoR = 2
   elif 0< R[0]<60:
       LoR = 1
   else:
       LoR= 0
   find_flag = 1
   Num = Finded_Num
   if LoR>0:
       FH = bytearray([0x2C,0x12,Num, LoR, find_flag, Find_Task,0x5B])
       uart.write(FH)
       print("识别到的数字是：", Num, "此数字所在方位：", LoR)
#----------------------------------- 功能函数定义 ------------------------------------------------


# part4
#----------------------------------- 主函数 ------------------------------------------------

while (True):
    Target_Num=3
    img = sensor.snapshot()
    UartReceiveDate()
    # 任务0 : 直走
    if Find_Task == 0:
        LoR=0
    # 任务1 : 识别目标数字
    if Find_Task == 1:
        temp=[FirstFindTemplate(img) for img in template_base]
        if not any(temp): continue
        [(roi,idx)]=[(returned_roi,i) for i,returned_roi in enumerate(temp) if returned_roi]
        FirstFindedNum(roi,idx+1)
    # 任务2 :
    elif Find_Task == 2:
        print(LoR)
        # Target_Num是目标数字
        if LoR==0:
            ROI_L=FindTemplateL(template_L[Target_Num-1])
            ROI_R=FindTemplateR(template_R[Target_Num-1])
            ROI_LL=FindTemplateLL(template_LL[Target_Num-1])
            ROI_RR=FindTemplateRR(template_RR[Target_Num-1])
        ROI=[ROI_L,ROI_R,ROI_LL,ROI_RR]
        if not LoR and any(ROI):
            print("temp2")
            Target_ROI=[roi for roi in ROI if roi][0]
            FindedNum(Target_ROI,Target_Num)

#----------------------------------- 主函数 ------------------------------------------------
