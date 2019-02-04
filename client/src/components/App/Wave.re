open BsReactNative;

type state =
  | Calm
  | Waxing
  | Waning;

type action =
  | Intensify
  | Abate
  | Stop;

let component = ReasonReact.reducerComponent("Wave");

let animatedValue = Animated.Value.create(0.0);

let duration = 1500.0;

let maxHeight = 300.0;

let animatedHeight =
  Animated.Value.interpolate(
    animatedValue,
    ~inputRange=[0.0, 1.0],
    ~outputRange=`float([0.0, maxHeight]),
    ~extrapolate=Animated.Interpolation.Clamp,
    (),
  );

let animation =
  Animated.timing(
    ~value=animatedValue,
    ~toValue=`raw(1.0),
    ~duration,
    ~easing=Easing.sin,
    (),
  );

let containerStyle =
  Style.(
    style([
      opacity(Animated(animatedValue)),
      height(Animated(animatedHeight)),
      flex(1.0),
      backgroundColor(String("limegreen")),
    ])
  );

let make = (~onFinish, _children) => {
  ...component,
  didMount: self => self.send(Intensify),

  initialState: () => Calm,
  reducer: (action, _state) =>
    switch (action) {
    | Intensify =>
      ReasonReact.UpdateWithSideEffects(
        Waxing,
        self =>
          Animated.start(
            Animated.timing(
              ~value=animatedValue,
              ~toValue=`raw(1.0),
              ~duration,
              (),
            ),
            ~callback=_ => self.send(Abate),
            (),
          ),
      )
    | Abate =>
      ReasonReact.UpdateWithSideEffects(
        Waning,
        self =>
          Animated.start(
            Animated.timing(
              ~value=animatedValue,
              ~toValue=`raw(0.0),
              ~duration=1500.0,
              (),
            ),
            ~callback=_ => self.send(Stop),
            (),
          ),
      )
    | Stop => ReasonReact.UpdateWithSideEffects(Calm, onFinish)
    },
  render: _self => <Animated.View style=containerStyle />,
};