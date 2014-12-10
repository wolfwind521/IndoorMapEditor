#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

class AbstractTool;

class ToolManager
{
public:
    ToolManager();

private:
    AbstractTool *m_tool;
};

#endif // TOOLMANAGER_H
