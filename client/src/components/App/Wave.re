open BsReactNative;

let component = ReasonReact.statelessComponent("Wave");

let animatedValue = Animated.Value.create(0.0);
let animation =
  Animated.timing(
    ~value=animatedValue,
    ~toValue=`raw(1.0),
    ~duration=100.0,
    (),
  );
let make = (~name, ~style, _children) => {
  ...component,
  didMount: _self => Animated.start(animation, ()),

  render: _self =>
    <View>
      <Text> {ReasonReact.string("Hello " ++ name ++ "!")} </Text>
    </View>,
};