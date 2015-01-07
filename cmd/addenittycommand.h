#ifndef ADDENITTYCOMMAND_H
#define ADDENITTYCOMMAND_H

#include <QUndoCommand>

class DocumentView;
class MapEntity;

class AddEnittyCommand : public QUndoCommand
{
public:
    explicit AddEnittyCommand(DocumentView *doc, MapEntity *entity, QUndoCommand *parent);
    void undo();
    void redo();

private:
    DocumentView *m_doc;
    MapEntity *m_entity;
};

#endif // ADDENITTYCOMMAND_H
