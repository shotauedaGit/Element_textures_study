#include "stdafx.h"
#include <iostream>

#include "MainForm.h"


using namespace OpenglOnCli;


//double buffering用のハック
//https://www.codeproject.com/Questions/226895/Double-buffered-painting-of-a-panel
public ref class MyPanel :  public System::Windows::Forms::Panel
{
public:
    void SetStyle(System::Windows::Forms::ControlStyles flag, System::Boolean value)
    {
        System::Windows::Forms::Control::SetStyle(flag, value);
    }
};


MainForm::MainForm(void)
{
  InitializeComponent();

  static_cast<MyPanel^>(m_main_panel)->SetStyle(ControlStyles::AllPaintingInWmPaint, true);
  static_cast<MyPanel^>(m_main_panel)->SetStyle(ControlStyles::DoubleBuffer, true);
}



System::Void MainForm::m_main_panel_Paint(
    System::Object^  sender, 
    System::Windows::Forms::PaintEventArgs^  e)
{
  //e->Grahicsを利用した描画
  //auto pen = gcnew System::Drawing::Pen(Drawing::Color::Red);
  //e->Graphics->DrawLine(pen, Drawing::Point(10,10), Drawing::Point(100,200) );
  //delete pen; //明示的なdeleteも可（ほっておけばガベコレが入る）


  //double bufferingを利用した描画//
  const int W = m_main_panel->Width ;
  const int H = m_main_panel->Height;
  auto pen = gcnew System::Drawing::Pen(Drawing::Color::Red);

  //0. bitmapを作成して，さらにpixelへのポインタも取得 
  Bitmap^ bmp = gcnew Bitmap(W,H,Imaging::PixelFormat::Format24bppRgb);
  System::Drawing::Rectangle rect = System::Drawing::Rectangle(0, 0, W,H);
    
  //1. pixelへ直接書き込み 
  Imaging::BitmapData^ bmpData = bmp->LockBits(rect, Imaging::ImageLockMode::ReadWrite, bmp->PixelFormat); 
  Byte* pBuf = (Byte*)bmpData->Scan0.ToPointer();

  for( int y=0; y < H/3; ++y) {
    for( int x=0; x < W/3; ++x) {
      pBuf[y*bmpData->Stride + x*3 + 0] = x; 
      pBuf[y*bmpData->Stride + x*3 + 1] = y; 
      pBuf[y*bmpData->Stride + x*3 + 2] = x+y; 
    }
  }
  
  //bmpへ図形を描画
  bmp->UnlockBits( bmpData );
  Graphics ^g = Graphics::FromImage( bmp );
  //draw background
  g->DrawLine   ( pen, 10,10, 100, 200); 
  g->DrawEllipse( pen, 50, 50, 100,150);

  //bmpをpanelに描画
  e->Graphics->InterpolationMode = Drawing2D::InterpolationMode::NearestNeighbor;
  e->Graphics->DrawImage(bmp, System::Drawing::Rectangle(0, 0,W,H), rect, GraphicsUnit::Pixel);

  delete bmp;
  delete pen;
}





System::Void MainForm::m_main_panel_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
  if (e->Button == System::Windows::Forms::MouseButtons::Left  ) std::cout << "left " << e->X << " " << e->Y << "\n"; 
  if (e->Button == System::Windows::Forms::MouseButtons::Middle) std::cout << "mid"   << e->X << " " << e->Y << "\n"; 
  if (e->Button == System::Windows::Forms::MouseButtons::Right ) std::cout << "right" << e->X << " " << e->Y << "\n"; 
}

System::Void MainForm::m_main_panel_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
}

System::Void MainForm::m_main_panel_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
{
}

System::Void MainForm::m_main_panel_Resize(System::Object^  sender, System::EventArgs^  e)
{
}

System::Void MainForm::MainForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
  std::cout << (int) e->KeyCode << "\n";
}


System::Void MainForm::MainForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
{
}
