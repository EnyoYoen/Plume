#include <QDateTime>
#include <QUrl>

typedef QPair<QDateTime, double> Timestamp;
typedef QPair<QString, QUrl> CalendarName;

enum class SpanType : quint8 {
    None,
    Day,
    NoWeekend,
    Week,
    Month,
};