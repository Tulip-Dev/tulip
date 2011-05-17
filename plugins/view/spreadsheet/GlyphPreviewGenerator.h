#ifndef GLYPHPREVIEWGENERATOR_H
#define GLYPHPREVIEWGENERATOR_H
#include <map>
#include <QtGui/QPixmap>
class GlyphPreviewGenerator
{
public:
    static GlyphPreviewGenerator & getInst();
    QPixmap getPreview(unsigned int pluginId);

private:
    GlyphPreviewGenerator();
    static GlyphPreviewGenerator* _instance;
    std::map<unsigned int,QPixmap> _previews;
};

class EdgeExtremityGlyphPreviewGenerator
{
public:
    static EdgeExtremityGlyphPreviewGenerator & getInst();
    QPixmap getPreview(unsigned int pluginId);

private:
    EdgeExtremityGlyphPreviewGenerator();
    static EdgeExtremityGlyphPreviewGenerator* _instance;
    std::map<unsigned int,QPixmap> _previews;
};

#endif // GLYPHPREVIEWGENERATOR_H
