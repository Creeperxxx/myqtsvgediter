//#include "drawtuxing.h"
//
//void drawtuxing::draw(QPainter* painter, tuxingjiedianparams* params)
//{
//	maketuxingjiedian(params)->draw(painter);
//}
//
//std::unique_ptr<Ituxingjiedian> drawtuxing::maketuxingjiedian(tuxingjiedianparams* params)
//{
//	if (!params)
//	{
//		throw std::runtime_error("error");//todo:except
//	}
//	else
//	{
//		ShapeType type = params->type;
//		switch (type)
//		{
//		default:
//		case ShapeType::juxing:
//			return std::make_unique<juxingjiedian>(params);
//			break;
//		case ShapeType::yuanxing:
//			return std::make_unique<yuanxingjiedian>(params);
//			break;
//		}
//	}
//}
//
