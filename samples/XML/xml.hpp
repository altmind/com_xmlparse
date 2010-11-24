#ifndef XML_HPP
#define XML_HPP

#include "xml_config.hpp"
#include <exception>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <wchar.h>
#include <setjmp.h>

#define IS_CHARTYPE(c, ct) (chartype_table[static_cast<unsigned char>(c)] & (ct))


namespace xml
{
    class out_of_memory : public std::exception
    {
        public:
            virtual const char * what() const throw()
            {
                return "Out of memory";
            }
    };

    class io_exception : public std::exception
    {
        public:
            virtual const char * what() const throw()
            {
                return "I/O error";
            }
    };

    class cannot_open_file : public io_exception
    {
        public:
            virtual const char * what() const throw()
            {
                return "Cannot open file";
            }
    };

    class xml_exception : public std::exception
    {
        virtual const char * what() const throw()
        {
            return "XML exception";
        }
    };

    class bad_pi : public xml_exception
    {

        virtual const char * what() const throw()
        {
            return "Bad PI";
        }
    };

    class bad_start_element : public xml_exception
    {
        const char * what() const throw()
        {
            return "Bad start element";
        }
    };

    class bad_end_element : public xml_exception
    {
        virtual const char * what() const throw()
        {
            return "Bad end element";
        }
    };

    class end_element_mismatch : public xml_exception
    {
        virtual const char * what() const throw()
        {
            return "End element mismatch";
        }
    };

    class bad_attribute : public xml_exception
    {
        virtual const char * what() const throw()
        {
            return "Bad attribute";
        }
    };

    class unrecognized_tag : public xml_exception
    {
        virtual const char * what() const throw()
        {
            return "Unrecognized tag";
        }
    };

    class bad_comment : public xml_exception
    {
        virtual const char * what() const throw()
        {
            return "Bad comment";
        }
    };

    class empty_file : public io_exception
    {
        virtual const char * what() const throw()
        {
            return "Empty file";
        }
    };



}

namespace xml
{
    typedef char char_t;
    typedef size_t uintptr_t;

    const unsigned char chartype_table[256] =
    {
        55,  0,   0,   0,   0,   0,   0,   0,      0,   12,  12,  0,   0,   63,  0,   0,   // 0-15
        0,   0,   0,   0,   0,   0,   0,   0,      0,   0,   0,   0,   0,   0,   0,   0,   // 16-31
        8,   0,   6,   0,   0,   0,   7,   6,      0,   0,   0,   0,   0,   96,  64,  0,   // 32-47
        64,  64,  64,  64,  64,  64,  64,  64,     64,  64,  192, 0,   1,   0,   48,  0,   // 48-63
        0,   192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 192, 192, 192, 192, 192, // 64-79
        192, 192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 0,   0,   16,  0,   192, // 80-95
        0,   192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 192, 192, 192, 192, 192, // 96-111
        192, 192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 0, 0, 0, 0, 0,           // 112-127

        192, 192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 192, 192, 192, 192, 192, // 128+
        192, 192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 192, 192, 192, 192, 192,
        192, 192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 192, 192, 192, 192, 192,
        192, 192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 192, 192, 192, 192, 192,
        192, 192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 192, 192, 192, 192, 192,
        192, 192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 192, 192, 192, 192, 192,
        192, 192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 192, 192, 192, 192, 192,
        192, 192, 192, 192, 192, 192, 192, 192,    192, 192, 192, 192, 192, 192, 192, 192
    };

    enum xml_node_type
    {
        node_null,
        node_document,
        node_element,
        node_pcdata,
        node_cdata,
        node_comment,
        node_pi,
        node_declaration
    };

    enum chartype_t
    {
        ct_parse_pcdata = 1,	// \0, &, \r, <
        ct_parse_attr = 2,		// \0, &, \r, ', "
        ct_parse_attr_ws = 4,	// \0, &, \r, ', ", \n, tab
        ct_space = 8,			// \r, \n, space, tab
        ct_parse_cdata = 16,	// \0, ], >, \r
        ct_parse_comment = 32,	// \0, -, >, \r
        ct_symbol = 64,			// Any symbol > 127, a-z, A-Z, 0-9, _, :, -, .
        ct_start_symbol = 128	// Any symbol > 127, a-z, A-Z, _, :
    };


    struct xml_attribute_struct
    {
        /// Default ctor
        xml_attribute_struct() : name(0), is_name_allocated(false), is_value_allocated(false), value(0), prev_attribute_c(0), next_attribute(0)
        {
        }

        bool is_name_allocated;
        bool is_value_allocated;

        uintptr_t header;

        char_t* name;
        char_t* value;

        xml_attribute_struct* prev_attribute_c;
        xml_attribute_struct* next_attribute;
    };

    struct xml_node_struct
    {
        xml_node_struct(xml_node_type type): header(type), is_name_allocated(false), is_value_allocated(false), parent(0), name(0), value(0), first_child(0), prev_sibling_c(0), next_sibling(0), first_attribute(0)
        {
        }


        bool is_name_allocated;
        bool is_value_allocated;

        uintptr_t header;

        xml_node_struct* parent;

        char_t * name;
        char_t * value;

        xml_node_struct * first_child;

        xml_node_struct * prev_sibling_c;
        xml_node_struct * next_sibling;

        xml_attribute_struct * first_attribute;
    };

    inline xml_node_struct* append_node_ll(xml_node_struct* node, xml_node_type type = node_element)
    {
        xml_node_struct* child = new xml_node_struct(type);
        if (!child) return 0;

        child->parent = node;

        xml_node_struct* first_child = node->first_child;

        if (first_child)
        {
            xml_node_struct* last_child = first_child->prev_sibling_c;

            last_child->next_sibling = child;
            child->prev_sibling_c = last_child;
            first_child->prev_sibling_c = child;
        }
        else
        {
            node->first_child = child;
            child->prev_sibling_c = child;
        }

        return child;
    }

    inline void destroy_attribute(xml_attribute_struct* a)
    {
        if (a->is_name_allocated) delete[] a->name;
        if (a->is_value_allocated) delete[] a->value;

        delete a;
    }

    inline void destroy_node(xml_node_struct* n)
    {
//        if (n->is_name_allocated) delete[] n->name;
   //     if (n->is_value_allocated) delete[] n->value;



        for (xml_attribute_struct* attr = n->first_attribute; attr; )
        {
            xml_attribute_struct* next = attr->next_attribute;

            destroy_attribute(attr);

            attr = next;
        }

        for (xml_node_struct* child = n->first_child; child; )
        {
            xml_node_struct* next = child->next_sibling;

            destroy_node(child);

            child = next;
        }
        delete n;
    }

    inline xml_attribute_struct* append_attribute_ll(xml_node_struct* node)
    {
        xml_attribute_struct * a = new xml_attribute_struct;
        if (!a) return 0;

        xml_attribute_struct* first_attribute = node->first_attribute;

        if (first_attribute)
        {
            xml_attribute_struct* last_attribute = first_attribute->prev_attribute_c;

            last_attribute->next_attribute = a;
            a->prev_attribute_c = last_attribute;
            first_attribute->prev_attribute_c = a;
        }
        else
        {
            node->first_attribute = a;
            a->prev_attribute_c = a;
        }

        return a;
    }

    class xml_parser
    {
        public:
            xml_parser() {}


#define PUSHNODE(TYPE)	{ cursor = append_node_ll(cursor, TYPE); if (!cursor) throw out_of_memory(); }
#define POPNODE()	{ cursor = cursor->parent; }
#define SKIPWS() { while (IS_CHARTYPE(*s, ct_space)) ++s; }
#define SCANFOR(X) { while (*s != 0 && !(X)) ++s; }
#define SCANWHILE(X) { while ((X)) ++s; }
#define ENDSEG() { ch = *s; *s = 0; ++s; }
#define ENDSWITH(c, e) ((c) == (e) || ((c) == 0))
#define CHECK_ERROR(err) { if (*s == 0) throw err; }

            void parse_question(char_t*& ref_s, xml_node_struct*& ref_cursor)
            {
                char_t* s = ref_s;
                xml_node_struct* cursor = ref_cursor;
                char_t ch = 0;


                // parse node contents, starting with question mark
                ++s;

                char_t* target = s;

                if (!IS_CHARTYPE(*s, ct_start_symbol)) throw bad_pi();

                SCANWHILE(IS_CHARTYPE(*s, ct_symbol));
                CHECK_ERROR(bad_pi());

                // determine node type; stricmp / strcasecmp is not portable
                bool declaration = (target[0] | ' ') == 'x' && (target[1] | ' ') == 'm' && (target[2] | ' ') == 'l' && target + 3 == s;

                if (declaration)
                {
                    // disallow non top-level declarations
                    if (cursor->header != node_document) throw bad_pi();
                    PUSHNODE(node_declaration);
                }
                else
                { 
                    PUSHNODE(node_pi);
                }

                cursor->name = target;

                ENDSEG();

                // parse value/attributes
                if (ch == '?')
                {
                    // empty node
                    if (!ENDSWITH(*s, '>')) throw bad_pi();
                    s += (*s == '>');

                    POPNODE();
                }
                else if (IS_CHARTYPE(ch, ct_space))
                {
                    SKIPWS();

                    // scan for tag end
                    char_t* value = s;

                    SCANFOR(s[0] == '?' && ENDSWITH(s[1], '>'));
                    CHECK_ERROR(bad_pi());

                    if (declaration)
                    {
                        // replace ending ? with / so that 'element' terminates properly
                        *s = '/';

                        // we exit from this function with cursor at node_declaration, which is a signal to parse() to go to LOC_ATTRIBUTES
                        s = value;
                    }
                    else
                    {
                        // store value and step over >
                        cursor->value = value;
                        POPNODE();

                        ENDSEG();

                        s += (*s == '>');
                    }
                }
                else throw bad_pi();

                // store from registers
                ref_s = s;
                ref_cursor = cursor;

            }

            void parse_comment(char_t*& ref_s, xml_node_struct* cursor)
            {
                char_t* s = ref_s;

                // parse node contents, starting with exclamation mark
                ++s;

                if (*s == '-') // '<!-...'
                {
                    ++s;

                    if (*s == '-') // '<!--...'
                    {
                        ++s;

                        // Scan for terminating '-->'.
                        SCANFOR(s[0] == '-' && s[1] == '-' && ENDSWITH(s[2], '>'));
                        CHECK_ERROR(bad_comment());
                        *s = 0; // Zero-terminate this segment at the first terminating '-'.

                         s += (s[2] == '>' ? 3 : 2); // Step over the '\0->'.

                    }
                    else throw bad_comment();
                }
                else if (*s == 0) throw bad_comment();
                else throw unrecognized_tag();

                ref_s = s;
            }



            void parse(char_t * s, xml_node_struct * xmldoc)
            {
                char_t ch = 0;
                xml_node_struct * cursor = xmldoc;
                char_t* mark = s;

                while (*s != 0)
                {
                    if (*s == '<')
                    {
                        ++s;
LOC_TAG:
                        if (IS_CHARTYPE(*s, ct_start_symbol)) // '<#...'
                        {
                            PUSHNODE(node_element);
                            cursor->name = s;

                            SCANWHILE(IS_CHARTYPE(*s, ct_symbol)); // Scan for a terminator.
                            ENDSEG(); // Save char in 'ch', terminate & step over.

                            if (ch == '>')
                            {
                                // end of tag
                            }
                            else if (IS_CHARTYPE(ch, ct_space))
                            {
LOC_ATTRIBUTES:
                                while (true)
                                {
                                    SKIPWS(); // Eat any whitespace.


                                    if (IS_CHARTYPE(*s, ct_start_symbol)) // <... #...
                                    {
                                        xml_attribute_struct* a = append_attribute_ll(cursor); // Make space for this attribute.
                                        if (!a) throw out_of_memory();

                                        a->name = s; // Save the offset.

                                        SCANWHILE(IS_CHARTYPE(*s, ct_symbol)); // Scan for a terminator.
                                        CHECK_ERROR(bad_attribute());

                                        ENDSEG(); // Save char in 'ch', terminate & step over.
                                        CHECK_ERROR(bad_attribute());

                                        if (IS_CHARTYPE(ch, ct_space))
                                        {
                                            SKIPWS(); // Eat any whitespace.
                                            CHECK_ERROR(bad_attribute());

                                            ch = *s;
                                            ++s;
                                        }


                                        if (ch == '=') // '<... #=...'
                                        {
                                            SKIPWS(); // Eat any whitespace.

                                            if (*s == '"' || *s == '\'') // '<... #="...'
                                            {
                                                ch = *s; // Save quote char to avoid breaking on "''" -or- '""'.
                                                ++s; // Step over the quote.
                                                a->value = s; // Save the offset.
                                                SCANWHILE(IS_CHARTYPE(*s, ct_symbol));

                                                ENDSEG();
                                                // if ()
                                                // s = strconv_attribute(s, ch);

                                                if (!s) throw bad_attribute();

                                                // After this line the loop continues from the start;
                                                // Whitespaces, / and > are ok, symbols and EOF are wrong,
                                                // everything else will be detected
                                                if (IS_CHARTYPE(*s, ct_start_symbol)) throw bad_attribute();
                                            }
                                            else throw bad_attribute();
                                        }
                                        else throw bad_attribute();
                                    }
                                    else if (*s == '/')
                                    {

                                        ++s;

                                        if (*s == '>')
                                        {

                                            POPNODE();

                                            s++;

                                            break;
                                        }
                                        else throw bad_start_element();
                                    }
                                    else if (*s == '>')
                                    {
                                        ++s;

                                        break;
                                    }
                                    else throw bad_start_element();
                                }
                            }
                            else if (ch == '/') // '<#.../'
                            {
                                if (!ENDSWITH(*s, '>')) throw bad_start_element();

                                POPNODE(); // Pop.
                                s += (*s == '>');

                            }
                            else throw bad_start_element();

                        }
                        else if (*s == '/')
                        {
                            ++s;

                            char_t* name = cursor->name;
                            if (!name) throw end_element_mismatch();

                            while (IS_CHARTYPE(*s, ct_symbol))
                            {
                                if (*s++ != *name++) throw end_element_mismatch();
                            }

                            if (*name)
                            {
                                throw end_element_mismatch();
                            }

                            POPNODE(); // Pop.

                            SKIPWS();


                            if (*s != '>')throw bad_end_element();
                            ++s;
                        }
                        else if (*s == '?') // '<?...'
                        {

                            parse_question(s, cursor);
                            if (cursor && cursor->header == node_declaration) goto LOC_ATTRIBUTES;
                        }
                        else if (*s = '!')
                        {
                            parse_comment(s, cursor);

                        }
                        else
                        {
                            //printf("%s\n\n\n\n", s);
                            throw unrecognized_tag();
                        }
                    }
                    else
                    {
                        mark = s; // Save this offset while searching for a terminator.
                        SKIPWS(); // Eat whitespace if no genuine PCDATA here.

                        if (*s == '<' || !*s)
                        {
                            continue;
                        }

                        if (cursor->parent)
                        {
                            //printf("%s\n\n\n\n", s);
                            PUSHNODE(node_pcdata);
                            cursor->value = s; // Save the offset.


                            SCANWHILE(*s != '<');
                            //ENDSEG();
                            --s;
                            *s = 0;
                            ++s;
                            ++s;
                           
                            POPNODE(); // Pop since this is a standalone.

                            if (!*s) break;

                        }
                        else
                        {
                            //printf("%s\n\n\n\n", s);

                            SCANFOR(*s == '<'); // '...<'
                            if (!*s) break;

                            ++s;
                        }
                        goto LOC_TAG;
                    }
                }
                correct_parse_results(xmldoc);
            }


            void correct_parse_results(xml_node_struct * xmldoc)
            {
                char_t* value = 0;
                xml_node_struct* i;
                i = xmldoc->first_child;
                do
                {

                    if (i->header == node_pcdata)
                    {
                        int next_length = strlen(i->value);
                        int old_length = 0;
                        if (xmldoc->value != 0) old_length = strlen(xmldoc->value);
                        int new_length = next_length + old_length;
                        value = new char_t[new_length];
                        *value = 0;
                        if (xmldoc->value != 0) 
                        {
                            strcpy(value, xmldoc->value);
                            delete[] xmldoc->value;
                        }
                        strcat(value, i->value);
                        xmldoc->value = value;
                        i->is_value_allocated = true;
                    }
                    if (i->first_child != 0) correct_parse_results(i);
                    i = i->next_sibling;

                } while (i != 0);
            }
    };

};

namespace xml
{
    class xml_writer;
    class xml_attribute;
    class xml_node_iterator;
    class xml_attribute_iterator;

    class xml_attribute
    {
        friend class xml_attribute_iterator;
        friend class xml_node;

        private:
        xml_attribute_struct* _attr;

        typedef xml_attribute_struct* xml_attribute::*unspecified_bool_type;

        public:
        // Default constructor. Constructs an empty attribute.
        xml_attribute();

        // Constructs attribute from internal pointer
        explicit xml_attribute(xml_attribute_struct* attr);

        // Check if attribute is empty
        bool empty() const;

        // Get attribute name/value, or "" if attribute is empty
        const char_t* name() const;
        const char_t* value() const;

        // Set attribute name/value (returns false if attribute is empty or there is not enough memory)
        bool set_name(const char_t* rhs);
        bool set_value(const char_t* rhs);

        // Get next/previous attribute in the attribute list of the parent node
        xml_attribute next_attribute() const;
        xml_attribute previous_attribute() const;
    };

    class xml_node
    {
        friend class xml_attribute_iterator;
        friend class xml_node_iterator;
        protected:
        xml_node_struct* _root;
        // Default constructor. Constructs an empty node.
        public:
        xml_node();

        // Constructs node from internal pointer
        explicit xml_node(xml_node_struct* p);

        // Check if node is empty.
        bool empty() const;

        // Get node type
        xml_node_type type() const;

        // Get node name/value, or "" if node is empty or it has no name/value
        const char_t* name() const;
        const char_t* value() const;

        // Get attribute list
        xml_attribute first_attribute() const;
        xml_attribute last_attribute() const;

        // Get children list
        xml_node first_child() const;
        xml_node last_child() const;

        // Get next/previous sibling in the children list of the parent node
        xml_node next_sibling() const;
        xml_node previous_sibling() const;

        // Get parent node
        xml_node parent() const;

        // Get root of DOM tree this node belongs to
        //xml_node root() const;

        // Get child, attribute or next/previous sibling with the specified name
        xml_node child(const char_t* name) const;
        xml_attribute attribute(const char_t* name) const;
        xml_node next_sibling(const char_t* name) const;
        xml_node previous_sibling(const char_t* name) const;

        // Get child value of current node; that is, value of the first child node of type PCDATA/CDATA
        const char_t* child_value() const;

        // Get child value of child with specified name. Equivalent to child(name).child_value().
        const char_t* child_value(const char_t* name) const;

        // Set node name/value (returns false if node is empty, there is not enough memory, or node can not have name/value)
        bool set_name(const char_t* rhs);
        bool set_value(const char_t* rhs);

        // Add attribute with specified name. Returns added attribute, or empty attribute on errors.
        xml_attribute append_attribute(const char_t* name);

        xml_node append_child(xml_node_type type = node_element);

        // Add child element with specified name. Returns added node, or empty node on errors.
        xml_node append_child(const char_t* name);

        // Remove specified attribute
        bool remove_attribute(const xml_attribute& a);
        bool remove_attribute(const char_t* name);

        // Remove specified child
        bool remove_child(const xml_node& n);
        bool remove_child(const char_t* name);

        typedef xml_node_iterator iterator;

        iterator begin() const;
        iterator end() const;

        typedef xml_attribute_iterator attribute_iterator;

        attribute_iterator attributes_begin() const;
        attribute_iterator attributes_end() const;
    };

    class xml_node_iterator
    {
        friend class xml_node;

        private:
        xml_node _wrap;
        xml_node _parent;

        xml_node_iterator(xml_node_struct* ref, xml_node_struct* parent);

        public:
        // Default constructor
        xml_node_iterator();

        // Construct an iterator which points to the specified node
        xml_node_iterator(const xml_node& node);

        // Iterator operators
        bool operator==(const xml_node_iterator& rhs) const;
        bool operator!=(const xml_node_iterator& rhs) const;

        xml_node& operator*();
        xml_node* operator->();

        const xml_node_iterator& operator++();
        xml_node_iterator operator++(int);

        const xml_node_iterator& operator--();
        xml_node_iterator operator--(int);
    };

    class xml_attribute_iterator
    {
        friend class xml_node;

        private:
        xml_attribute _wrap;
        xml_node _parent;

        xml_attribute_iterator(xml_attribute_struct* ref, xml_node_struct* parent);

        public:

        // Default constructor
        xml_attribute_iterator();

        // Construct an iterator which points to the specified attribute
        xml_attribute_iterator(const xml_attribute& attr, const xml_node& parent);

        // Iterator operators
        bool operator==(const xml_attribute_iterator& rhs) const;
        bool operator!=(const xml_attribute_iterator& rhs) const;

        xml_attribute& operator*();
        xml_attribute* operator->();

        const xml_attribute_iterator& operator++();
        xml_attribute_iterator operator++(int);

        const xml_attribute_iterator& operator--();
        xml_attribute_iterator operator--(int);
    };

    class xml_document: public xml_node
    {
        friend class xml_writer;
        private:
            char_t* contents;

            void create();
            void destroy();
            void reset();
        public:
            xml_document();
            ~xml_document();
            void load_file(const char* path);
            void load_buffer(char_t* contents);
           };


    class xml_writer
    {
    public:
        xml_writer(xml_document& doc_) : doc(doc_)
        {
        }

        void write_to_file(const char * file_name)
        {
            FILE * f = fopen(file_name, "w");
            write_node(f, doc._root);
            fclose(f);
        }
    private:

        void write_node(FILE * f, xml_node_struct * node, int indent = 0)
        {

                xml_node_struct* i;
                i = node->first_child;
                do
                {
                    if (i->header == node_declaration)
                    {
                        for (int j = 0; j < indent; j++) fprintf(f, " ");
                        if (i->name)
                        {
                            fprintf(f, "<?%s", i->name);
                            xml_attribute_struct * a = i->first_attribute;
                            while (a)
                            {
                                fprintf(f, " %s=\"%s\"", a->name, a->value);
                                a = a->next_attribute;
                            }
                            fprintf(f, " ?>\n");

                        }

                    }
                    else if (i->header == node_element)
                    {
                        for (int j = 0; j < indent; j++) fprintf(f, " ");
                        if (i->name)
                        {
                            if (i->first_child || i->value)
                            {
                                if (i->first_attribute)
                                {
                                    fprintf(f, "<%s", i->name);
                                    xml_attribute_struct * a = i->first_attribute;
                                    while (a)
                                    {
                                        fprintf(f, " %s=\"%s\"", a->name, a->value);
                                        a = a->next_attribute;
                                    }
                                    fprintf(f, ">\n");
                                }
                                else fprintf(f, "<%s>\n", i->name);
                                if (i->value != 0)
                                {
                                   // printf("%s\n\n\n", i->value);
                                    fprintf(f, "%s\n", i->value);
                                }
                            }
                            else
                            {
                                fprintf(f, "<%s", i->name);
                                if (i->first_attribute)
                                {
                                    xml_attribute_struct * a = i->first_attribute;
                                    while (a)
                                    {
                                        fprintf(f, " %s=\"%s\"", a->name, a->value);
                                        a = a->next_attribute;
                                    }
                                }
                                fprintf(f, "/>\n", i->name);

                            }
                        }
                    }


                    if (i->first_child != 0) write_node(f, i, indent+4);

                    if (i->header == node_element)
                    {
                        if (i->name)
                        {
                            if (i->first_child)
                            {
                                for (int j = 0; j < indent; j++) fprintf(f, " ");
                                fprintf(f, "</%s>\n", i->name);

                            }
                        }
                    }

                    i = i->next_sibling;

                } while (i != 0);

        }
        xml_document& doc;
    };



};
#endif
