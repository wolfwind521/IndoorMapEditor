#include "feature.h"
#include <QJsonArray>
#include <QDateTime>
#include <QRandomGenerator>
#include <QTextStream>

Feature::Feature(QGraphicsItem *parent) :
    QGraphicsObject(parent), m_type("0")
{
    generateId();
}

Feature::Feature(const QString &name, QGraphicsItem *parent) :
    QGraphicsObject(parent), m_type("0")
{
    generateId();
    setObjectName(name);
}

const QString &Feature::brief() const
{
    return m_brief;
}

int Feature::generateId(){
    //利用时间生成6位id, 小概率生成同样id...
    // m_id = QDateTime::currentDateTime().toTime_t() % 1000000 +  qrand()% 100000 + 1000000;
    // m_id = QDateTime::currentDateTime().toSecsSinceEpoch() % 1000000 + std::rand() % 100000 + 1000000;
    m_id = QDateTime::currentDateTime().toSecsSinceEpoch() % 1000000 + QRandomGenerator::global()->bounded(100000) + 1000000;
    return m_id;
}

void Feature::setBrief(const QString & brief)
{
    if(m_brief == brief)
        return;
    m_brief = brief;
    emit briefChanged(m_brief);
}

bool Feature::isClassOf(const QString &className) const {
    QString myClassName;
    myClassName = this->metaObject()->className();
    return (!myClassName.compare(className));
}
int Feature::id() const
{
    return m_id;
}

void Feature::setId(const int id)
{
    if(m_id == id)
        return;
    int oldid = m_id;
    m_id = id;
    emit idChanged(oldid, m_id);
}

const QString & Feature::enName() const
{
    return m_enName;
}

void Feature::setEnName(const QString & name){
    if( m_enName == name)
        return;
    m_enName = name;
    emit enNameChanged(m_enName);
}

const QPointF & Feature::center() const
{
    return m_center;
}

void Feature::setCenter(const QPointF & center)
{
    if(m_center == center)
        return;
    m_center = center;
    emit centerChanged(m_center);
}

const QString & Feature::type() {
    return m_type;
}

void Feature::setType(const QString & type){
    if(m_type == type)
        return;
    m_type = type;
    emit typeChanged(m_type);
}

void Feature::setParentFeature(Feature *feature) {
    setParent(feature);
    setParentItem(feature);
}

bool Feature::load(const QJsonObject &jsonObject)
{
    setObjectName( jsonObject["Name"].toString() );
    m_enName = jsonObject["Name_en"].toString();
    m_brief = jsonObject["Brief"].toString();

    QTextStream out(stdout);
    out << jsonObject["Name"].toString() << Qt::endl;

    const QJsonArray & jsonArray = jsonObject["Center"].toArray();
    if(jsonArray.size() == 2){
        m_center = QPointF(jsonArray[0].toDouble(), -jsonArray[1].toDouble());
    }
    return true;
}

bool Feature::save(QJsonObject &jsonObject) const
{
    jsonObject["Name"] = objectName();
    jsonObject["Name_en"] = m_enName;
    jsonObject["Brief"] = m_brief;
    return true;
}

QRectF Feature::boundingRect() const
{
    return QRectF(m_center, m_center);
}

void Feature::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

void Feature::transformFeature(const QTransform &matrix){
    m_center = matrix.map(m_center);
    emit centerChanged(m_center);
}
