
#ifndef CONFXML_H
#define CONFXML_H

#include <gloox/parser.h>
#include <gloox/taghandler.h>

using namespace gloox;

#include <string>

/**
 * @brief 配置文件类。继承自TagHandler,用这个类可以使用Parser类，
 * 给它喂Xml文件的feed可以自动调用handleTag来使Xml类解析为Tag类
 */

class ConfXml : public TagHandler
{

public:
        ConfXml(): tagxml(0)
        {
                parser = new Parser(this);
        }

        /**通过调用得到XML转化为Tag类，并拷贝到本类的tagxml里*/
        void handleTag(Tag* tag)
        {
                tagxml = tag->clone();
        }

        /**将本类的Tag设置为给定的Tag类*/
        void setTagXml(Tag* tag_)
        {
                tagxml = tag_->clone();
        }

        /**得到本类的Tag*/
        Tag* getTagXml()
        {
                return tagxml;
        }

        /**得到本类的Tag，既从XML文件转化得来 的Tag*/
        Tag* xml_from_file(char* filename);
        /**将本类的Tag写进文件filename里，以XML的格式*/
        void xml_saveto_file(char* filename);

private:
        Tag* tagxml;
        Parser* parser;
};








#endif
