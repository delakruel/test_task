run: make
./test_task _ARG_
-----
Варианты ответа карты для тестов:
-----
6F29840E325041592E5359532E4444463031A517BF0C1461129F2A038106438701814F07A00000065810109000

6F10840E325041592E5359532E44444630319000

6F20840E325041592E5359532E4444463031A50EBF0C0B61099F2A038106438701019000

6F37840E325041592E5359532E4444463031A525BF0C2261129F2A038106438701034F07A0000004320001610C8701024F07A00000065810109000

6F39840E325041592E5359532E4444463031A527BF0C2461129F2A038106438701034F07A0000004320001610E8701024F09A000000658101033339000

6F33840E325041592E5359532E4444463031A521BF0C1E61068701034F07A0000004320001610E8701024F09A000000658101033339000  // без тега 9F2A

6F23840E325041592E5359532E4444463031A511BF0C0E610C4F07A00000000310108701019000 // без тега 9F2A, приложение не поддерживаемое

6F6C840E325041592E5359532E4444463031A55ABF0C5761094F07A000000658200161134F08A0000006581000039F2A03810643870153610C8701014F07A000000003101061124F07A00000065820028701109F2A0381064361134F08A0000006581000029F2A038106438701529000 // разные приоритеты и их отсутствие, вперемешку с неподдерживаемыми
-----
При выполнении задания полагался в основном на данные варианты для отладки - т.к. по идее, теги 'A5', '61', '4F' являются одними из обязательных тегов ответа карты на пакет SELECT PPSE, но я не могу пока знать всех нюансов -
https://www.emvco.com/wp-content/uploads/2017/05/BookB_Entry_Point_Specification_v2_6_20160809023257319.pdf стр. 22, табл. 3-2
В дополнение к предыдущему, в теге '61' парсятся только теги '9F2A', '4F', '87', хотя есть еще и другие, опциональные/комбинируемые (можно легко добавить).
