#ifndef EXTENDEDCONFIG_H
#define EXTENDEDCONFIG_H

#include <QObject>
/**
 * 由编译时传入的参数EX_CONFIG获取配置信息，传入的参数为json格式并已转换为16进制字符串
 * 该类为单例，每次调用instance方法获取配置信息对象
 * 示例：QVariant appName = ExtendedConfig::instance()->get("appName")
 */
class ExtendedConfig : public QObject
{
  Q_OBJECT
public:
  static ExtendedConfig *instance();
  /**
   * 获取所有传入的配置信息
   */
  QVariantMap *getConfigs();
  /**
   * 获取指定的配置信息，需使用QVariant.isValid()判断返回的配置是否存在
   */
  QVariant get(const QString &key);

private:
  explicit ExtendedConfig(QObject *parent = 0);
  ExtendedConfig(const ExtendedConfig &) Q_DECL_EQ_DELETE;
  ExtendedConfig &operator=(ExtendedConfig config) Q_DECL_EQ_DELETE;

  void init();

  static ExtendedConfig *_instance;

  QVariantMap *config;
};

#endif // EXTENDEDCONFIG_H
