open BsReactNative;

let component = ReasonReact.statelessComponent("Wave");

let animatedValue = Animated.Value.create(0.0);

let animatedHeight =
  Animated.Value.interpolate(
    animatedValue,
    ~inputRange=[0.0, 1.0],
    ~outputRange=`float([0.0, 100.0]),
    ~extrapolate=Animated.Interpolation.Clamp,
    (),
  );

let animation =
  Animated.timing(
    ~value=animatedValue,
    ~toValue=`raw(1.0),
    ~duration=1000.0,
    ~easing=Easing.ease,
    (),
  );

let containerStyle =
  Style.(
    style([
      opacity(Animated(animatedValue)),
      height(Animated(animatedHeight)),
      flex(1.0),
      backgroundColor(String("lightgreen")),
    ])
  );

let make = _children => {
  ...component,
  didMount: _self =>
    Animated.start(
      Animated.timing(
        ~value=animatedValue,
        ~toValue=`raw(1.0),
        ~duration=1000.0,
        (),
      ),
      ~callback=_didFinish => Js.log("ok"),
      (),
    ),
  render: _self => <Animated.View style=containerStyle />,
};