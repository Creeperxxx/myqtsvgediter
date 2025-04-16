#pragma once

// QtCFConstants.h

namespace qtcf {

    // tuxingku 图形库相关常量
    namespace tuxingku {

        namespace diagramwidget {
            constexpr auto backgroundcolor = "tuxingku.diagramwidget.backgroundcolor";
            constexpr auto issizefix = "tuxingku.diagramwidget.issizefix";
            constexpr auto widgetradio = "tuxingku.diagramwidget.widgetradio";
            constexpr auto fixwidth = "tuxingku.diagramwidget.fixwidth";
            constexpr auto fixheight = "tuxingku.diagramwidget.fixheight";
            constexpr auto maxwidth = "tuxingku.diagramwidget.maxwidth";
            constexpr auto maxheight = "tuxingku.diagramwidget.maxheight";
            constexpr auto minwidth = "tuxingku.diagramwidget.minwidth";
            constexpr auto minheight = "tuxingku.diagramwidget.minheight";

            namespace pen {
                constexpr auto width = "tuxingku.diagramwidget.pen.width";
                constexpr auto color = "tuxingku.diagramwidget.pen.color";
            }

            constexpr auto brush = "tuxingku.diagramwidget.brush";
            constexpr auto isdrawbypainter = "tuxingku.diagramwidget.isdrawbypainter";
        } // namespace diagramwidget

    } // namespace tuxingku

    // huabu 画布相关常量
    namespace huabu {
        constexpr auto backgroundcolor = "huabu.backgroundcolor";
        constexpr auto width = "huabu.width";
        constexpr auto height = "huabu.height";

        namespace tuxingspace {
            constexpr auto spacewidth = "huabu.tuxingspace.spacewidth";
            constexpr auto spaceheight = "huabu.tuxingspace.spaceheight";

            namespace pen {
                constexpr auto width = "huabu.tuxingspace.pen.width";
                constexpr auto color = "huabu.tuxingspace.pen.color";
            }

            constexpr auto brush = "huabu.tuxingspace.brush";
        }
    } // namespace huabu

    // 其他顶层常量
    constexpr auto mimetype = "mimetype";

    // tuxing 图形相关常量
    namespace tuxing {

        namespace rectangle {
            constexpr auto radio = "tuxing.rectangle.radio";
            constexpr auto drawbypainter = "tuxing.rectangle.drawbypainter";
            constexpr auto drawbyloadpic = "tuxing.rectangle.drawbyloadpic";
            constexpr auto imagepath = "tuxing.rectangle.imagepath";
        } // namespace rectangle

        namespace circle {
            constexpr auto drawbypainter = "tuxing.circle.drawbypainter";
            constexpr auto drawbyloadpic = "tuxing.circle.drawbyloadpic";
            constexpr auto imagepath = "tuxing.circle.imagepath";
            constexpr auto boundingrectradio = "tuxing.circle.boundingrectradio";
        } // namespace circle

        namespace triangle {

            namespace edgeradio {
                constexpr auto bottom = "tuxing.triangle.edgeradio.bottom";
                constexpr auto left = "tuxing.triangle.edgeradio.left";
                constexpr auto right = "tuxing.triangle.edgeradio.right";
            }

            constexpr auto edgetype = "tuxing.triangle.edgetype";
            constexpr auto totate = "tuxing.triangle.totate";
            constexpr auto drawbypainter = "tuxing.triangle.drawbypainter";
            constexpr auto drawbyloadpic = "tuxing.triangle.drawbyloadpic";
            constexpr auto imagepath = "tuxing.triangle.imagepath";
        } // namespace triangle

    } // namespace tuxing

} // namespace qtcf




//constexpr const char* imagepathyuanxing = ":/tuxingku/yuanxing.png";
//constexpr const char* mymimetype = "application/x-custom-shape";
//constexpr float diagramitemradio = 1;	
//constexpr int diagramitemwidth = 50;
//constexpr int diagramitemheight = diagramitemwidth / diagramitemradio;
//constexpr bool isdiagramitemsizefix = false;
//constexpr int diagramitemmaxwidth = 40;
//constexpr int diagramitemmaxheight = diagramitemmaxwidth;
//constexpr int diatramitemminwidth = 20;
//constexpr int diatramitemminheight = diatramitemminwidth;
//constexpr Qt::GlobalColor diagramitempencolor = Qt::black;
//constexpr int diagramitempenwidth = 2;
//constexpr Qt::GlobalColor diagramitembrush = Qt::transparent;
//constexpr Qt::GlobalColor huabutuxingpencolor = Qt::black;
//constexpr int huabutuxingpenwidth = 2;
//constexpr Qt::GlobalColor huabutuxingbrush = Qt::transparent;
//constexpr Qt::GlobalColor huabupencolor = Qt::black;
//constexpr int huabupenwidth = 2;
//constexpr Qt::GlobalColor huabubrush = Qt::transparent;
//constexpr int huabutuxingwith = 100;
//constexpr float huabutuxingradio = 1;
//constexpr int huabutuxingheight = huabutuxingwith / huabutuxingradio;
//constexpr float huabujuxingradio = 2;
//
////tuxing.h
//constexpr const char* imagepathjuxing = ":/tuxingku/juxing.png";
//constexpr float diagramitemjuxingradio = 2;
//constexpr Qt::GlobalColor diagramitembackgroundcolor = Qt::white;
//
////huabu.h
//constexpr Qt::GlobalColor huabubackgroundcolor = Qt::white;
