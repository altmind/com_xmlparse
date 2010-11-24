#include "xml.hpp"

namespace xml
{

    size_t get_file_size(FILE* file)
    {

        fseek(file, 0, SEEK_END);
        size_t length = ftell(file);
        fseek(file, 0, SEEK_SET);

        // check for I/O errors
        if (length < 0) throw io_exception();

        return length;
    }

    inline bool allow_insert_child(xml_node_type parent, xml_node_type child)
    {
        if (parent != node_document && parent != node_element) return false;
        if (child == node_document || child == node_null) return false;
        if (parent != node_document && (child == node_declaration/* || child == node_doctype*/)) return false;

        return true;
    }

    xml_node::xml_node() : _root(0)
    {

    }

    xml_node::xml_node(xml_node_struct* p): _root(p)
    {
    }

    xml_node::iterator xml_node::begin() const
    {
        return iterator(_root ? _root->first_child : 0, _root);
    }

    xml_node::iterator xml_node::end() const
    {
        return iterator(0, _root);
    }

    xml_node::attribute_iterator xml_node::attributes_begin() const
    {
        return attribute_iterator(_root ? _root->first_attribute : 0, _root);
    }

    xml_node::attribute_iterator xml_node::attributes_end() const
    {
        return attribute_iterator(0, _root);
    }

    bool xml_node::empty() const
    {
        return !_root;
    }

    const char_t* xml_node::name() const
    {
        return (_root && _root->name) ? _root->name : "";
    }

    xml_node_type xml_node::type() const
    {
        //return node_element;
        return static_cast<xml_node_type>(_root->header);
    }

    const char_t* xml_node::value() const
    {
        return (_root && _root->value) ? _root->value : "";
    }

    xml_node xml_node::child(const char_t* name) const
    {
        if (!_root) return xml_node();

        for (xml_node_struct* i = _root->first_child; i; i = i->next_sibling)
            if (i->name && strcmp(name, i->name) == 0) return xml_node(i);

        return xml_node();
    }


    xml_attribute xml_node::attribute(const char_t* name) const
    {
        if (!_root) return xml_attribute();

        for (xml_attribute_struct* i = _root->first_attribute; i; i = i->next_attribute)
            if (i->name && strcmp(name, i->name) == 0)
                return xml_attribute(i);

        return xml_attribute();
    }

    xml_node xml_node::next_sibling(const char_t* name) const
    {
        if (!_root) return xml_node();

        for (xml_node_struct* i = _root->next_sibling; i; i = i->next_sibling)
            if (i->name && strcmp(name, i->name) == 0) return xml_node(i);

        return xml_node();
    }

    xml_node xml_node::next_sibling() const
    {
        if (!_root) return xml_node();

        if (_root->next_sibling) return xml_node(_root->next_sibling);
        else return xml_node();
    }

    xml_node xml_node::previous_sibling(const char_t* name) const
    {
        if (!_root) return xml_node();

        for (xml_node_struct* i = _root->prev_sibling_c; i->next_sibling; i = i->prev_sibling_c)
            if (i->name && strcmp(name, i->name) == 0) return xml_node(i);

        return xml_node();
    }

    xml_node xml_node::previous_sibling() const
    {
        if (!_root) return xml_node();

        if (_root->prev_sibling_c->next_sibling) return xml_node(_root->prev_sibling_c);
        else return xml_node();
    }

    xml_node xml_node::parent() const
    {
        return _root ? xml_node(_root->parent) : xml_node();
    }

    const char_t* xml_node::child_value() const
    {
        if (!_root) return "";

        for (xml_node_struct* i = _root->first_child; i; i = i->next_sibling)
        {
            xml_node_type type = static_cast<xml_node_type>(i->header)/* static_cast<xml_node_type>((i->header & xml_memory_page_type_mask) + 1)*/;

            if (i->value && (type == node_pcdata || type == node_cdata))
                return i->value;
        }

        return "";
    }

    const char_t* xml_node::child_value(const char_t* name) const
    {
        return child(name).child_value();
    }

    xml_attribute xml_node::first_attribute() const
    {
        return _root ? xml_attribute(_root->first_attribute) : xml_attribute();
    }

    xml_attribute xml_node::last_attribute() const
    {
        return _root && _root->first_attribute ? xml_attribute(_root->first_attribute->prev_attribute_c) : xml_attribute();
    }

    xml_node xml_node::first_child() const
    {
        return _root ? xml_node(_root->first_child) : xml_node();
    }

    xml_node xml_node::last_child() const
    {
        return _root && _root->first_child ? xml_node(_root->first_child->prev_sibling_c) : xml_node();
    }

    bool xml_node::set_name(const char_t* rhs)
    {
        switch (type())
        {
        case node_pi:
        case node_declaration:
        case node_element:
            {
            char_t * name = new char_t(strlen(rhs));
            if (_root->is_name_allocated) delete[] _root->name;
            _root->is_name_allocated = true;
            _root->name = name;
            strcpy(name, rhs);
            return true;
            }
        default:
          return false;
        }
    }

    bool xml_node::set_value(const char_t* rhs)
    {
        switch (type())
        {
            case node_pi:
            case node_cdata:
            case node_pcdata:
            case node_comment:
                {
                char_t * value = new char_t(strlen(rhs));
                if (_root->is_value_allocated) delete[] _root->value;
                _root->is_value_allocated = true;
                _root->value = value;
                strcpy(value, rhs);
                return true;
                }
            default:
                return false;
        }
    }

    xml_attribute xml_node::append_attribute(const char_t* name)
    {
        if (type() != node_element && type() != node_declaration) return xml_attribute();

        xml_attribute a(append_attribute_ll(_root));
        a.set_name(name);

        return a;
    }

    xml_node xml_node::append_child(xml_node_type type)
    {
        if (!allow_insert_child(this->type(), type)) return xml_node();

        xml_node n(append_node_ll(_root, type));

        if (type == node_declaration) n.set_name("xml");

        return n;
    }

    xml_node xml_node::append_child(const char_t* name)
    {
        xml_node result = append_child(node_element);

        result.set_name(name);

        return result;
    }

    bool xml_node::remove_attribute(const char_t* name)
    {
        return remove_attribute(attribute(name));
    }

    bool xml_node::remove_attribute(const xml_attribute& a)
    {
        if (!_root || !a._attr) return false;

        // check that attribute belongs to *this
        xml_attribute_struct* attr = a._attr;

        while (attr->prev_attribute_c->next_attribute) attr = attr->prev_attribute_c;

        if (attr != _root->first_attribute) return false;

        if (a._attr->next_attribute) a._attr->next_attribute->prev_attribute_c = a._attr->prev_attribute_c;
        else if (_root->first_attribute) _root->first_attribute->prev_attribute_c = a._attr->prev_attribute_c;

        if (a._attr->prev_attribute_c->next_attribute) a._attr->prev_attribute_c->next_attribute = a._attr->next_attribute;
        else _root->first_attribute = a._attr->next_attribute;

        destroy_attribute(a._attr);

        return true;
    }

    bool xml_node::remove_child(const char_t* name)
    {
        return remove_child(child(name));
    }

    bool xml_node::remove_child(const xml_node& n)
    {
        if (!_root || !n._root || n._root->parent != _root) return false;

        if (n._root->next_sibling) n._root->next_sibling->prev_sibling_c = n._root->prev_sibling_c;
        else if (_root->first_child) _root->first_child->prev_sibling_c = n._root->prev_sibling_c;

        if (n._root->prev_sibling_c->next_sibling) n._root->prev_sibling_c->next_sibling = n._root->next_sibling;
        else _root->first_child = n._root->next_sibling;

        destroy_node(n._root);

        return true;
    }





    xml_node_iterator::xml_node_iterator()
    {
    }

    xml_node_iterator::xml_node_iterator(const xml_node& node): _wrap(node), _parent(node.parent())
    {
    }

    xml_node_iterator::xml_node_iterator(xml_node_struct* ref, xml_node_struct* parent): _wrap(ref), _parent(parent)
    {
    }

    bool xml_node_iterator::operator==(const xml_node_iterator& rhs) const
    {
        return _wrap._root == rhs._wrap._root && _parent._root == rhs._parent._root;
    }

    bool xml_node_iterator::operator!=(const xml_node_iterator& rhs) const
    {
        return _wrap._root != rhs._wrap._root || _parent._root != rhs._parent._root;
    }

    xml_node& xml_node_iterator::operator*()
    {
        return _wrap;
    }

    xml_node* xml_node_iterator::operator->()
    {
        return &_wrap;
    }

    const xml_node_iterator& xml_node_iterator::operator++()
    {
        _wrap._root = _wrap._root->next_sibling;
        return *this;
    }

    xml_node_iterator xml_node_iterator::operator++(int)
    {
        xml_node_iterator temp = *this;
        ++*this;
        return temp;
    }

    const xml_node_iterator& xml_node_iterator::operator--()
    {
        _wrap = _wrap._root ? _wrap.previous_sibling() : _parent.last_child();
        return *this;
    }

    xml_node_iterator xml_node_iterator::operator--(int)
    {
        xml_node_iterator temp = *this;
        --*this;
        return temp;
    }



    xml_attribute_iterator::xml_attribute_iterator()
    {
    }

    xml_attribute_iterator::xml_attribute_iterator(const xml_attribute& attr, const xml_node& parent): _wrap(attr), _parent(parent)
    {
    }

    xml_attribute_iterator::xml_attribute_iterator(xml_attribute_struct* ref, xml_node_struct* parent): _wrap(ref), _parent(parent)
    {
    }

    bool xml_attribute_iterator::operator==(const xml_attribute_iterator& rhs) const
    {
        return _wrap._attr == rhs._wrap._attr && _parent._root == rhs._parent._root;
    }

    bool xml_attribute_iterator::operator!=(const xml_attribute_iterator& rhs) const
    {
        return _wrap._attr != rhs._wrap._attr || _parent._root != rhs._parent._root;
    }

    xml_attribute& xml_attribute_iterator::operator*()
    {
        return _wrap;
    }

    xml_attribute* xml_attribute_iterator::operator->()
    {
        return &_wrap;
    }

    const xml_attribute_iterator& xml_attribute_iterator::operator++()
    {
        _wrap._attr = _wrap._attr->next_attribute;
        return *this;
    }

    xml_attribute_iterator xml_attribute_iterator::operator++(int)
    {
        xml_attribute_iterator temp = *this;
        ++*this;
        return temp;
    }

    const xml_attribute_iterator& xml_attribute_iterator::operator--()
    {
        _wrap = _wrap._attr ? _wrap.previous_attribute() : _parent.last_attribute();
        return *this;
    }

    xml_attribute_iterator xml_attribute_iterator::operator--(int)
    {
        xml_attribute_iterator temp = *this;
        --*this;
        return temp;
    }




    xml_attribute::xml_attribute(): _attr(0)
    {
    }

    xml_attribute::xml_attribute(xml_attribute_struct* attr): _attr(attr)
    {
    }

    xml_attribute xml_attribute::next_attribute() const
    {
        return _attr ? xml_attribute(_attr->next_attribute) : xml_attribute();
    }

    xml_attribute xml_attribute::previous_attribute() const
    {
        return _attr && _attr->prev_attribute_c->next_attribute ? xml_attribute(_attr->prev_attribute_c) : xml_attribute();
    }

    bool xml_attribute::empty() const
    {
        return !_attr;
    }

    const char_t* xml_attribute::name() const
    {
        return (_attr && _attr->name) ? _attr->name : "";
    }

    const char_t* xml_attribute::value() const
    {
        return (_attr && _attr->value) ? _attr->value : "";
    }

    bool xml_attribute::set_name(const char_t* rhs)
    {
        if (!_attr) return false;

        if (_attr->is_name_allocated) delete[] _attr->name;
        _attr->name = new char_t[strlen(rhs)];
        strcpy(_attr->name, rhs);

        return true;
    }

    bool xml_attribute::set_value(const char_t* rhs)
    {
        if (!_attr) return false;

        if (_attr->is_value_allocated) delete[] _attr->value;
        _attr->value = new char_t[strlen(rhs)];
        strcpy(_attr->value, rhs);

        return true;
    }



    xml_document::xml_document(): contents(0)
    {
        create();
    }

    xml_document::~xml_document()
    {
        destroy();
    }


    void xml_document::create()
    {
        _root = new xml_node_struct(node_document);
    }

    void xml_document::destroy()
    {
         destroy_node(_root);
    }

    void xml_document::reset()
    {
        destroy();
        create();
    }


    void xml_document::load_file(const char* path)
    {
        reset();

        FILE* file = fopen(path, "rb");

        if (!file) throw cannot_open_file();

        // get file size
        size_t size = get_file_size(file);

        if (size == 0) throw empty_file();

        // allocate buffer for the whole file
        contents = new char_t[size];

        if (!contents)
        {
            fclose(file);
            if (!file) throw out_of_memory();
        }

        // read file in memory
        size_t read_size = fread(contents, 1, size, file);
        fclose(file);

        if (read_size != size)
        {
            delete[] contents;
            throw io_exception();
        }


        load_buffer(contents);

    }

    void xml_document::load_buffer(char_t* contents)
    {
        reset();
        // parse

        xml_parser parser;
        parser.parse(contents, _root);

    }
};
