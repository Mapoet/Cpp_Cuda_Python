//#define BOOST_PYTHON_STATIC_LIB
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/python.hpp>
#include <boost/python/module.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/operators.hpp>
#include <boost/python/dict.hpp>
#include "json.hpp"
namespace obp
{
    using namespace std;
    namespace bp = boost::python;
    namespace bn = bp::numpy;

     template <typename... Args>
     std::string operator%(std::string fmt, std::tuple<Args...> args)
     {
         char line[800];
         sprintf(line, fmt.c_str(), args);
         return line;
     }
    char const *greet()
    {
        // string str;
        // auto a = "%d-%d" % std::tuple<int, int>(2, 3);
        return "hello, world";
    }
    struct World
    {
        void set(string msg) { this->msg = msg; }
        string greet() { return msg; }
        string msg;
    };

    class ArrayHandler
    {
    public:
        ArrayHandler();
        bp::object Generate();
        bp::object GeneratebyTuple(bp::object &data);
        bp::object GeneratebyList(bp::object &data);
        bp::object Reshape(bp::object &data);
        void ShowData(bp::object &data);
        void DataType(bp::object &data);
    };

    ArrayHandler::ArrayHandler()
    {
    }

    bp::object ArrayHandler::Generate()
    {
        bp::tuple shape = bp::make_tuple(4, 4);
        bn::dtype type = bn::dtype::get_builtin<float>();
        bn::ndarray newArray = bn::zeros(shape, type);
        //bn::ndarray newArray = bn::empty(shape, type);
        return newArray;
    }

    bp::object ArrayHandler::GeneratebyTuple(bp::object &data)
    {
        bp::tuple dataList = (bp::tuple)data;
        bn::ndarray newArray = bn::array(dataList);
        return newArray;
    }

    bp::object ArrayHandler::GeneratebyList(bp::object &data)
    {
        bp::list dataList = (bp::list)data;
        bn::ndarray newArray = bn::array(dataList);
        return newArray;
    }

    bp::object ArrayHandler::Reshape(bp::object &data)
    {
        bn::ndarray dataArray = bn::from_object(data);
        for (int i = 0; i < dataArray.get_nd(); i++)
        {
            std::cout << "Size of Dim" << i + 1 << ": " << dataArray.get_shape()[i] << std::endl;
        }
        bp::tuple newShape = bp::make_tuple(2, 2, 2, 2);
        bn::ndarray newArray = dataArray.reshape(newShape);

        return newArray;
    }

    void ArrayHandler::ShowData(bp::object &data)
    {
        std::cout << "Original Array：" << bp::extract<char const *>(bp::str(data)) << std::endl;
        bn::ndarray dataArray = bn::from_object(data);
        data = dataArray.reshape(bp::make_tuple(16));
        std::cout << "Reshaped Array：" << bp::extract<char const *>(bp::str(data)) << std::endl;
    }

    void ArrayHandler::DataType(bp::object &data)
    {
        bn::ndarray dataArray = bn::from_object(data);
        std::cout << "Datatype is " << bp::extract<char const *>(bp::str(dataArray.get_dtype())) << std::endl;
    }
    template<class T>
    struct _point:bp::object{
        public:
        T _x,_y;
        _point():_x(),_y(){}
        _point(bp::object obj){_x=((_point<T>)obj)._x;_y=((_point<T>)obj)._y;}
        _point( T x, T y):_x((T)x),_y((T)y){}
        T abs(){return std::sqrt(_x*_x+_y*_y);}
        std::string to_json(){
           nlohmann::json j1;
           j1["x"]=_x;
           j1["y"]=_y;
           stringstream ss;
           ss<<j1;
           return ss.str();
        }
      //  T& x(){return _x;}
      //  T& y(){return _y;}
/*
        friend _point operator+(_point x,_point y){
            return _point(x._x+y._x,x._y+y._y);
        }
        friend _point operator-(_point x,_point y){
            return _point(x._x-y._x,x._y-y._y);
        } */
    };
    /*
    struct point_add:bp::detail::binary_op<bp::detail::op_add>{

    };*/
    template<class T>
    using point=struct _point<T>;
    template<class T>
    struct _line:bp::object{
        point<T> _s;
        point<T> _e;
        _line():_s(),_e(){}
        _line(bp::object obj){_s=((_line<T>)obj)._s;_e=((_line<T>)obj)._e;}
        _line( point<T> s, point<T> e):_s(s),_e(e){}
        point<T> dif(){return point<T>(_e._x-_s._x,_e._y-_s._y);}
        T abs(){return dif().abs();}
        std::string to_json(){
           nlohmann::json j1;
           stringstream ss;
           j1["s"]["x"]=_s._x;
           j1["s"]["y"]=_s._y;
           j1["e"]["x"]=_e._x;
           j1["e"]["y"]=_e._y;
           ss<<j1;
           return ss.str();
        }
    };
    template<class T>
    using line=struct _line<T>;

    line<double> from_dict(bp::object&obj){
        double sx=0,sy=0,ex=0,ey=0;
        bp::dict dict=(bp::dict)(obj);
        auto s=bp::extract<bp::dict>(dict["s"]);
        if(s.check()){
            bp::dict xy=s();
            sx=bp::extract<double>(xy["x"]);
            sy=bp::extract<double>(xy["y"]);
        }
        auto e=bp::extract<bp::dict>(dict["e"]);
        if(e.check()){
            bp::dict xy=e();
            ex=bp::extract<double>(xy["x"]);
            ey=bp::extract<double>(xy["y"]);
        }
        return line<double>(point<double>(sx,sy),point<double>(ex,ey));
    }
    line<double> from_json(string str){
        double sx=0,sy=0,ex=0,ey=0;
        nlohmann::json j1=nlohmann::json::parse(str);
        sx=j1["s"]["x"];
        sy=j1["s"]["y"];
        ex=j1["e"]["x"];
        ey=j1["e"]["y"];
        return line<double>(point<double>(sx,sy),point<double>(ex,ey));
    }
} // namespace obp

BOOST_PYTHON_MODULE(cpp_example) //导出的module 名字
{
    obp::bn::initialize();
    obp::bp::def("greet", obp::greet);
    obp::bp::class_<obp::World>("World")
        .def("greet", &obp::World::greet)
        .def("set", &obp::World::set);

    //bp::def("SetDictValue", SetDictValue);
    obp::bp::class_<obp::ArrayHandler>("ArrayHandler", obp::bp::init<>())
        .def("Generate", &obp::ArrayHandler::Generate)
        .def("GeneratebyTuple", &obp::ArrayHandler::GeneratebyTuple)
        .def("GeneratebyList", &obp::ArrayHandler::GeneratebyList)
        .def("ShowData", &obp::ArrayHandler::ShowData)
        .def("DataType", &obp::ArrayHandler::DataType)
        .def("Reshape", &obp::ArrayHandler::Reshape);
    obp::bp::class_<obp::point<double>>("point",obp::bp::init<>())
        .def(obp::bp::init<double,double>())
        .def_readwrite("x",&obp::point<double>::_x)
        .def_readwrite("y",&obp::point<double>::_y)
        .def_readonly("abs",&obp::point<double>::abs)
        .def("to_json",&obp::point<double>::to_json);
    obp::bp::class_<obp::line<double>>("line",obp::bp::init<>())
        .def(obp::bp::init<obp::point<double>,obp::point<double>>())
        .def_readwrite("s",&obp::line<double>::_s)
        .def_readwrite("e",&obp::line<double>::_e)
        .def_readonly("dif",&obp::line<double>::dif)
        .def_readonly("abs",&obp::line<double>::abs)
        .def("to_json",&obp::line<double>::to_json);
    obp::bp::def("from_dict",&obp::from_dict);
    obp::bp::def("from_json",&obp::from_json);
}
#if 0
int main(){
    Py_Initialize();
    PyRun_SimpleString("import helloworld");
    PyRun_SimpleString("helloworld.printHello()");
    Py_Finalize();
}
#endif