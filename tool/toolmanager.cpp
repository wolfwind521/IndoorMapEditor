#include "toolmanager.h"
#include <QObject>

ToolManager* ToolManager::m_instance = NULL;

ToolManager::ToolManager():m_tool(NULL)
{
}

ToolManager::~ToolManager(){
    delete m_tool;
    m_tool = NULL;
}

ToolManager* ToolManager::instance(){
    if(m_instance == NULL)
        m_instance = new ToolManager();
    return m_instance;
}

AbstractTool & ToolManager::currentTool() const{
    return *m_tool;
}

void ToolManager::setTool(AbstractTool *tool){
    if(m_tool != tool){
        delete m_tool;
        m_tool = tool;
    }
}
