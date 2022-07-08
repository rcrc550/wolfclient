#include "usershow.h"
#include "ui_usershow.h"

UserShow::UserShow(QWidget *parent) :
    QWidget(parent), m_defaultImg(":/images/lf3.png"),
    ui(new Ui::UserShow)
{
    ui->setupUi(this);
    connect( &m_timer , SIGNAL(timeout())
             , this ,SLOT( slot_checkTimer() ) );

    m_lastTime = QTime::currentTime();

    m_timer.start( 1000 );
    id=-1;
}

UserShow::~UserShow()
{
    delete ui;
}

void UserShow::set_info(int id, QString username)
{
    m_id=id;
    m_username=username;
    QString st=QString("用户名：%1").arg(username);
    ui->lb_name->setText(st);

}
#include<QPainter>
//重绘
void UserShow::paintEvent(QPaintEvent *event)
{
    //画黑色背景
    QPainter pianer(this);
    pianer.setBrush(Qt::black);//设置黑色画刷
    pianer.drawRect(0,0,this->width(),this->height());//画矩形
    //画视频帧
    //缩放
    if( m_img.size().height()<= 0  ) return;

    m_img = m_img.scaled(this->width() ,
                         this->height() - ui->lb_name->height() , Qt::KeepAspectRatio );

    QPixmap pix =QPixmap::fromImage( m_img );
    int x = this->width() - pix.width();
    x = x/2;
    int y = this->height() - pix.height() - ui->lb_name->height() ;
    y = ui->lb_name->height() + y/2 ;

    pianer.drawPixmap( QPoint( x , y ) , pix);  //用画笔把图片画出来了

    pianer.end();

}

void UserShow::slot_setImage(QImage &img)
{
    m_img=img;
    //每次获得画面更新时间
    m_lastTime = QTime::currentTime();
    this->update();//触发重绘

}

void UserShow::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    Q_EMIT SIG_itemCliked(m_id,m_username);

}

void UserShow::slot_checkTimer()
{
    //定时检测 接收界面是否超时
    if( m_lastTime.secsTo( QTime::currentTime() ) > 5 )
    {
        //设为默认 画面
        slot_setImage ( m_defaultImg );
    }
}
