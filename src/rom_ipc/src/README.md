အဓိက သတိပြုရမည့် အချက်များ
၁။ use_intra_process_comms(true): ဒါဟာ Switch အဖွင့်အပိတ်လိုပါပဲ။ ဒီ option မပါရင် Node နှစ်ခုဟာ process တစ်ခုတည်းမှာ ရှိနေရင်တောင် ပုံမှန် network layer ကနေပဲ ဒေတာဖြတ်ပို့ပါလိမ့်မယ်။

၂။ std::unique_ptr: ဒေတာကို ပို့တဲ့သူက "ပိုင်ဆိုင်ခွင့် (Ownership)" ကို စွန့်လွှတ်လိုက်တာ ဖြစ်ပါတယ်။ ဒါမှ ROS 2 Middleware က ဒီ memory address ကို လက်ခံမယ့်သူဆီ တိုက်ရိုက်ပေးနိုင်မှာပါ။

၃။ Single Executor: Node နှစ်ခုလုံးဟာ process တစ်ခုတည်းမှာ run နေရပါမယ်။ တကယ်လို့ terminal နှစ်ခုခွဲပြီး run ရင် IPC အလုပ်မလုပ်တော့ဘဲ ပုံမှန် DDS mode ကို ပြန်ပြောင်းသွားမှာ ဖြစ်ပါတယ်။

Production အတွက် ဘယ်လိုသုံးမလဲ?
သင်လုပ်နေတဲ့ Robot မှာ Node တွေ အများကြီး ရှိလာတဲ့အခါ main() function ထဲမှာ node တွေကို manual ထည့်မယ့်အစား ROS 2 Components (Shared Libraries) အနေနဲ့ ရေးသင့်ပါတယ်။ ပြီးမှ ros2 run rclcpp_components component_container ကို သုံးပြီး လိုအပ်တဲ့ node တွေကို container တစ်ခုတည်းထဲ စုထည့်လိုက်ရင် IPC ရဲ့ အကျိုးကျေးဇူးကို အပြည့်အဝ ရရှိမှာ ဖြစ်ပါတယ်။