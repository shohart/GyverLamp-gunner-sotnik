// Если вы хотите добавить эффекты или сделать им копии для демонстрации на разных настройках, нужно делать это в 3 местах (помимо добавления программы самого эффекта ):
// 0. в файле effects.ino - добавляется программный код самого эффекта.
// 2. в файле Constants.h - придумываются названия "EFF_......" и задаются порядковые номера эффектам. В конце указывается общее количество MODE_AMOUNT.
//    Не изменяйте порядковые номера у эффектов с 0U по 15U. У них иное управление скоростью.
// 3. там же в файле Constants.h ниже - задаётся Реестр эффектов для передачи в приложение. Он живёт отдельно. 
//    Если у вас приложение не поддерживает запрос реестра у лампы, реестр можно не менять.
// 4. в файле effectTicker.ino - подключается процедура вызова эффекта на соответствующий ей "EFF_......"
//    Можно подключать один и тот же эффект под разными номерами. Например EFF_FIRE (1U), EFF_FIRE2 (26U), EFF_FIRE3 (27U). Будет три огня разных цветов.

uint32_t effTimer;

void effectsTick()
{
  if (!dawnFlag)
  {
    if (ONflag && (millis() - effTimer >= ((currentMode > 15U ) ? 256U - modes[currentMode].Speed : (currentMode < 10U ) ? 50 : 15))) // у эффектов до 15U бегунок Скорость не регулирует задержку
    {
      effTimer = millis();
      switch (currentMode)
      {
        case EFF_WHITE_COLOR:         whiteColorStripeRoutine();          break;  // ( 0U) Белый свет
        case EFF_MADNESS:             madnessNoiseRoutine();              break;  // ( 1U) Безумие
        case EFF_CLOUDS:              cloudsNoiseRoutine();               break;  // ( 2U) Облака
        case EFF_LAVA:                lavaNoiseRoutine();                 break;  // ( 3U) Лава
        case EFF_PLASMA:              plasmaNoiseRoutine();               break;  // ( 4U) Плазма
        case EFF_RAINBOW:             rainbowNoiseRoutine();              break;  // ( 5U) Радуга 3D
        case EFF_RAINBOW_STRIPE:      rainbowStripeNoiseRoutine();        break;  // ( 6U) Павлин
        case EFF_ZEBRA:               zebraNoiseRoutine();                break;  // ( 7U) Зебра
        case EFF_FOREST:              forestNoiseRoutine();               break;  // ( 8U) Лес
        case EFF_OCEAN:               oceanNoiseRoutine();                break;  // ( 9U) Океан
        case EFF_BBALLS:              BBallsRoutine(true);                break;  // (10U) Мячики
        case EFF_BBALLS_WHITE:        BBallsRoutine(false);               break;  // (11U) Белые мячики
        case EFF_BBALLS_TRACES:       BBallsRoutine(true);                break;  // (12U) Мячики со шлейфом
        case EFF_SPIRO:               spiroRoutine();                     break;  // (13U) Спирали
        case EFF_SINUSOID3:           Sinusoid3Routine();                 break;  // (14U) Синусоид
        case EFF_METABALLS:           MetaBallsRoutine();                 break;  // (15U) Метаболз


        case EFF_FIRE:                fireRoutine(true);                  break;  // (16U) Огонь
        case EFF_FIRE_WHITTE:         fireRoutine(true);                  break;  // (17U) Белый огонь
        case EFF_FIRE_BLUE:           fireRoutine(true);                  break;  // (18U) Голубой огонь
        case EFF_FIRE_GREEN:          fireRoutine(true);                  break;  // (19U) Зелёный огонь
        case EFF_MATRIX:              matrixRoutine();                    break;  // (20U) Матрица
        case EFF_WATERFALL:           fire2012WithPalette();              break;  // (21U) Водопад
        case EFF_WATERFALL_WHITE:     fire2012WithPalette();              break;  // (22U) Белый водопад
        case EFF_WATERFALL_4IN1:      fire2012WithPalette4in1();          break;  // (23U) Водопад 4 в 1
        case EFF_PULSE_SLOW:          pulseRoutine(2U);                   break;  // (24U) Медленный пульс
        case EFF_PULSE_FAST:          pulseRoutine(1U);                   break;  // (25U) Быстрый пульс
        case EFF_PULSE_RAINBOW:       pulseRoutine(4U);                   break;  // (26U) Радужный пульс
        case EFF_PULSE_WHITE:         pulseRoutine(8U);                   break;  // (27U) Белый пульс
        case EFF_COMET:               RainbowCometRoutine();              break;  // (28U) Комета
        case EFF_COMET_WHITE:         ColorCometRoutine();                break;  // (29U) Белая комета
        case EFF_COMET_COLOR:         ColorCometRoutine();                break;  // (30U) Одноцветная комета
        case EFF_COMET_PULSING:       MultipleStream4();                  break;  // (31U) Пульсирующая комета
        case EFF_COMET_TWO:           MultipleStream();                   break;  // (32U) Две кометы
        case EFF_COMET_THREE:         MultipleStream2();                  break;  // (33U) Три кометы


        case EFF_FIREFLY:             MultipleStream3();                  break;  // (34U) Парящий огонь
        case EFF_FIREFLY_TOP:         MultipleStream5();                  break;  // (35U) Верховой огонь
        case EFF_SNAKE:               MultipleStream8();                  break;  // (36U) Радужный змей
        case EFF_RAIN:                RainRoutine();                      break;  // (37U) Цветной дождь
        case EFF_SPARKLES:            sparklesRoutine();                  break;  // (38U) Конфетти
        case EFF_RAINBOW_VER:         rainbowVerticalRoutine();           break;  // (39U) Радуга вертикальная
        case EFF_RAINBOW_HOR:         rainbowHorizontalRoutine();         break;  // (40U) Радуга горизонтальная
        case EFF_RAINBOW_DIAG:        rainbowDiagonalRoutine();           break;  // (41U) Радуга диагональная
        case EFF_POOL:                poolRoutine();                      break;  // (42U) Бассейн
        case EFF_SNOW:                snowRoutine();                      break;  // (43U) Снегопад
        case EFF_SNOWSTORM:           snowStormRoutine();                 break;  // (44U) Метель
        case EFF_STARFALL:            starfallRoutine();                  break;  // (45U) Звездопад
        case EFF_LIGHTERS:            lightersRoutine();                  break;  // (46U) Светлячки
        case EFF_LIGHTER_TRACES:      ballsRoutine();                     break;  // (47U) Светлячки со шлейфом
        case EFF_PAINTBALL:           lightBallsRoutine();                break;  // (48U) Пейнтбол
        case EFF_CUBE:                ballRoutine();                      break;  // (49U) Блуждающий кубик
        case EFF_COLORS:              colorsRoutine();                    break;  // (50U) Смена цвета
        case EFF_COLOR:               colorRoutine();                     break;  // (51U) Цвет
        case EFF_TEXT:                text_running();                     break;  // (52U) Бегущая строка
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
