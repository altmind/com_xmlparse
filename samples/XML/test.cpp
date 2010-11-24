#include "xml.hpp"
#include <iostream>
#include <stdio.h>


using namespace std;

int main()
{
    xml::xml_parser parser;
    xml::xml_node_struct * xml = new xml::xml_node_struct(xml::node_document);

    try
    {
        xml::xml_document doc;
        doc.load_file("/home/miksayer/Projects/XML/xml.xml");
       // for (xml::xml_node::iterator itr = doc.child("root").begin(); itr != doc.child("root").end(); itr++)
      //      cout << itr->attribute("attr").value() << endl;
      //  cout << doc.child("root").value() << endl;
doc.child("root").set_name("hui");
//doc.remove_child("hui");
//doc.child("hui").child("el").append_child("el3").append_attribute("hui").set_value("ui");       
    xml::xml_writer xml_w(doc);
// cout << doc.child("root").value() << endl;

        xml_w.write_to_file("/home/miksayer/Projects/XML/xml2.xml");
 //cout << doc.child("root").value() << endl;

        //cout << doc.child("root").child("element").attribute("attr").value() << endl;
    }
    catch (std::exception &e)
    {
        cout << e.what() << endl;
    }
    return 0;
}
