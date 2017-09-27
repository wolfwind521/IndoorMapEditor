#ifndef ADDENITTYCOMMAND_H
#define ADDENITTYCOMMAND_H

#include <QUndoCommand>

class DocumentView;
class Feature;

class AddEnittyCommand : public QUndoCommand
{
public:
    explicit AddEnittyCommand(DocumentView *doc, Feature *feature, QUndoCommand *parent);
    void undo();
    void redo();

private:
    DocumentView *m_doc;
    Feature *m_feature;
};

#endif // ADDENITTYCOMMAND_H
