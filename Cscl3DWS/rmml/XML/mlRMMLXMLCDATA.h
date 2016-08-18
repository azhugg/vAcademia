#pragma once

namespace rmml {

/**
 * JS-����� ��� CDATA XML-��������� RMML
 */

class mlXMLCDATASection :	
					public mlIXMLNode,
					public mlJSClass
{
friend class mlXMLDocument;
public:
	mlXMLCDATASection(void);
	void destroy() { MP_DELETE_THIS; }
	~mlXMLCDATASection(void);
MLJSCLASS_DECL2(mlXMLCDATASection)
IXMLNODE_IMPL
private:
	enum {
		JSPROP_data,
		JSPROP_length
	};
};

}
