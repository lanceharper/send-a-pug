open BsReactNative;

let component = ReasonReact.statelessComponent("Wave");

let animatedValue = Animated.Value.create(0.0);

let animation =
  Animated.timing(
    ~value=animatedValue,
    ~toValue=`raw(1.0),
    ~duration=1000.0,
    (),
  );

let containerStyle =
  Style.(
    style([
      height(Pt(450.)),
      opacity(Animated(animatedValue)),
      flex(1.0),
      backgroundColor(String("orange")),
    ])
  );

let make = (~name, _children) => {
  ...component,
  didMount: _self => {
    Animated.start(
      Animated.timing(
        ~value=animatedValue,
        ~toValue=`raw(1.0),
        ~duration=1000.0,
        (),
      ),
      ~callback=_didFinish => Js.log("ok"),
      (),
    );
  },

  render: _self =>
    <Animated.View style=containerStyle>
      <Text> {ReasonReact.string("Hello " ++ name ++ "!")} </Text>
    </Animated.View>,
};