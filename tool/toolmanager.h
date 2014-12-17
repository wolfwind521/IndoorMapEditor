#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

class AbstractTool;

class ToolManager
{
public:

    ~ToolManager();
    static ToolManager* instance();
    AbstractTool & currentTool() const;
    void setTool(AbstractTool* tool);
private:
    AbstractTool *m_tool;
    ToolManager();
    static ToolManager *m_instance;
};

#endif // TOOLMANAGER_H
