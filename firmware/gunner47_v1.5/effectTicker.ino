// Если вы хотите добавить эффекты или сделать им копии для демонстрации на разных настройках, нужно делать это в 5 местах:
// 1. в файле effects.ino - добавляется программный код самого эффекта.
// 2. в файле Constants.h - придумываются названия "EFF_......" и задаются порядковые номера эффектам. В конце указывается общее количество MODE_AMOUNT.
// 3. там же в файле Constants.h ниже - задаётся Реестр эффектов для передачи в приложение. 
//    Он живёт отдельно.  Если у вас приложение не поддерживает запрос списка эффектов у лампы, реестр можно не менять.
// 4. там же в файле Constants.h ещё ниже - задаётся Массив настроек эффектов по умолчанию.
//    Просто добавьте строчку для своего нового эффекта в нужное место. Это тоже не обязательно.
// 5. в файле effectTicker.ino - подключается процедура вызова эффекта на соответствующий ей "EFF_......"
//    Можно подключать один и тот же эффект под разными номерами. Например EFF_FIRE (1U), EFF_FIRE2 (26U), EFF_FIRE3 (27U). Будет три огня разных цветов.
// Для удобства изменения всех этих списков и настроек в архиве с прошивкой есть файл "таблица_эффектов.xls". В одном месте меняете - везде меняется.


uint32_t effTimer;

void effectsTick()
{
  if (!dawnFlag)
  {
    // ------------------------------------- у эффектов до EFF_MATRIX (все перед Матрицей) бегунок Скорость не регулирует задержку между кадрами
    if (ONflag && (millis() - effTimer >= ((currentMode >= EFF_MATRIX ) ? 256U - modes[currentMode].Speed : (currentMode < 12U ) ? 50 : 15)))
    {
      effTimer = millis();
      switch (currentMode)
      {
        case EFF_WHITE_COLOR:         whiteColorStripeRoutine();          break;  // ( 0U) Белый свет
        case EFF_COLOR:               colorRoutine();                     break;  // ( 1U) Цвет
        case EFF_COLORS:              colorsRoutine2();                   break;  // ( 2U) Смена цвета
        case EFF_MADNESS:             madnessNoiseRoutine();              break;  // ( 3U) Безумие
        case EFF_CLOUDS:              cloudsNoiseRoutine();               break;  // ( 4U) Облака
        case EFF_LAVA:                lavaNoiseRoutine();                 break;  // ( 5U) Лава
        case EFF_PLASMA:              plasmaNoiseRoutine();               break;  // ( 6U) Плазма
        case EFF_RAINBOW:             rainbowNoiseRoutine();              break;  // ( 7U) Радуга 3D
        case EFF_RAINBOW_STRIPE:      rainbowStripeNoiseRoutine();        break;  // ( 8U) Павлин
        case EFF_ZEBRA:               zebraNoiseRoutine();                break;  // ( 9U) Зебра
        case EFF_FOREST:              forestNoiseRoutine();               break;  // (10U) Лес
        case EFF_OCEAN:               oceanNoiseRoutine();                break;  // (11U) Океан
        case EFF_BBALLS:              BBallsRoutine();                    break;  // (12U) Мячики
        case EFF_BBALLS_TRACES:       BBallsRoutine();                    break;  // (13U) Мячики со шлейфом
        case EFF_BALLS_BOUNCE:        bounceRoutine();                    break;  // (14U) Мячики без границ
        case EFF_SPIRO:               spiroRoutine();                     break;  // (15U) Спирали
        case EFF_PRISMATA:            PrismataRoutine();                  break;  // (16U) Призмата
        case EFF_FLOCK:               flockRoutine(false);                break;  // (17U) Стая
        case EFF_FLOCK_N_PR:          flockRoutine(true);                 break;  // (18U) Стая и хищник
        case EFF_SINUSOID3:           Sinusoid3Routine();                 break;  // (19U) Синусоид
        case EFF_METABALLS:           MetaBallsRoutine();                 break;  // (20U) Метаболз


        case EFF_MATRIX:              matrixRoutine();                    break;  // (21U) Матрица
        case EFF_FIRE_2012:           fire2012again();                    break;  // (22U) Огонь 2012
        case EFF_FIRE_2018:           Fire2018_2();                       break;  // (23U) Огонь 2018
        case EFF_FIRE:                fireRoutine(true);                  break;  // (24U) Огонь
        case EFF_FIRE_WHITTE:         fireRoutine(true);                  break;  // (25U) Белый огонь
        case EFF_FIRE_GREEN:          fireRoutine(true);                  break;  // (26U) Цветной огонь
        case EFF_WHIRL:               whirlRoutine(true);                 break;  // (27U) Вихри пламени
        case EFF_WHIRL_MULTI:         whirlRoutine(false);                break;  // (28U) Разноцветные вихри
        case EFF_WATERFALL:           fire2012WithPalette();              break;  // (29U) Водопад
        case EFF_WATERFALL_WHITE:     fire2012WithPalette();              break;  // (30U) Белый водопад
        case EFF_WATERFALL_4IN1:      fire2012WithPalette4in1();          break;  // (31U) Водопад 4 в 1
        case EFF_POOL:                poolRoutine();                      break;  // (32U) Бассейн
        case EFF_PULSE_SLOW:          pulseRoutine(2U);                   break;  // (33U) Медленный пульс
        case EFF_PULSE_FAST:          pulseRoutine(1U);                   break;  // (34U) Быстрый пульс
        case EFF_PULSE_RAINBOW:       pulseRoutine(4U);                   break;  // (35U) Радужный пульс
        case EFF_PULSE_WHITE:         pulseRoutine(8U);                   break;  // (36U) Белый пульс
        case EFF_COMET:               RainbowCometRoutine();              break;  // (37U) Комета
        case EFF_COMET_WHITE:         ColorCometRoutine();                break;  // (38U) Белая комета
        case EFF_COMET_COLOR:         ColorCometRoutine();                break;  // (39U) Одноцветная комета
        case EFF_COMET_PULSING:       MultipleStream4();                  break;  // (40U) Пульсирующая комета
        case EFF_COMET_TWO:           MultipleStream();                   break;  // (41U) Две кометы
        case EFF_COMET_THREE:         MultipleStream2();                  break;  // (42U) Три кометы


        case EFF_FIREFLY:             MultipleStream3();                  break;  // (43U) Парящий огонь
        case EFF_FIREFLY_TOP:         MultipleStream5();                  break;  // (44U) Верховой огонь
        case EFF_SNAKE:               MultipleStream8();                  break;  // (45U) Радужный змей
        case EFF_SPARKLES:            sparklesRoutine();                  break;  // (46U) Конфетти
        case EFF_TWINKLES:            twinklesRoutine();                  break;  // (47U) Мерцание
        case EFF_WAVES:               WaveRoutine();                      break;  // (48U) Волны
        case EFF_RINGS:               ringsRoutine();                     break;  // (49U) Кодовый замок
        case EFF_CUBE2D:              cube2dRoutine();                    break;  // (50U) Кубик Рубика
        case EFF_SIMPLE_RAIN:         simpleRain();                       break;  // (51U) Тучка в банке
        case EFF_STORMY_RAIN:         stormyRain();                       break;  // (52U) Гроза в банке
        case EFF_COLOR_RAIN:          coloredRain();                      break;  // (53U) Осадки
        case EFF_RAIN:                RainRoutine();                      break;  // (54U) Разноцветный дождь
        case EFF_SNOW:                snowRoutine();                      break;  // (55U) Снегопад
        case EFF_SNOWSTORM:           stormRoutine2(false);               break;  // (56U) Метель
        case EFF_STARFALL:            stormRoutine2(true);                break;  // (57U) Звездопад
        case EFF_LIGHTERS:            lightersRoutine();                  break;  // (58U) Светлячки
        case EFF_LIGHTER_TRACES:      ballsRoutine();                     break;  // (59U) Светлячки со шлейфом
        case EFF_PAINTBALL:           lightBallsRoutine();                break;  // (60U) Пейнтбол
        case EFF_RAINBOW_VER:         rainbowVerticalRoutine();           break;  // (61U) Радуга вертикальная
        case EFF_RAINBOW_HOR:         rainbowHorizontalRoutine();         break;  // (62U) Радуга горизонтальная
        case EFF_RAINBOW_DIAG:        rainbowDiagonalRoutine();           break;  // (63U) Радуга диагональная
        case EFF_CUBE:                ballRoutine();                      break;  // (64U) Блуждающий кубик
        case EFF_TEXT:                text_running();                     break;  // (65U) Бегущая строка
      }
      FastLED.show();
    }
  }
}

void changePower()
{
  if (ONflag)
  {
    effectsTick();
    for (uint8_t i = 0U; i < modes[currentMode].Brightness; i = constrain(i + 8, 0, modes[currentMode].Brightness))
    {
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
    FastLED.setBrightness(modes[currentMode].Brightness);
    delay(2);
    FastLED.show();
  }
  else
  {
    effectsTick();
    for (uint8_t i = modes[currentMode].Brightness; i > 0; i = constrain(i - 8, 0, modes[currentMode].Brightness))
    {
      FastLED.setBrightness(i);
      delay(1);
      FastLED.show();
    }
    FastLED.clear();
    delay(2);
    FastLED.show();
  }

  #if defined(MOSFET_PIN) && defined(MOSFET_LEVEL)          // установка сигнала в пин, управляющий MOSFET транзистором, соответственно состоянию вкл/выкл матрицы
  digitalWrite(MOSFET_PIN, ONflag ? MOSFET_LEVEL : !MOSFET_LEVEL);
  #endif

  TimerManager::TimerRunning = false;
  TimerManager::TimerHasFired = false;
  TimerManager::TimeToFire = 0ULL;
  #ifdef AUTOMATIC_OFF_TIME      
    if (ONflag){
      TimerManager::TimerRunning = true;
      TimerManager::TimeToFire = millis() + AUTOMATIC_OFF_TIME;
    }
  #endif    
  
  if (FavoritesManager::UseSavedFavoritesRunning == 0U)     // если выбрана опция Сохранять состояние (вкл/выкл) "избранного", то ни выключение модуля, ни выключение матрицы не сбрасывают текущее состояние (вкл/выкл) "избранного"
  {
      FavoritesManager::TurnFavoritesOff();
  }

  #if (USE_MQTT)
  if (espMode == 1U)
  {
    MqttManager::needToPublish = true;
  }
  #endif
}
