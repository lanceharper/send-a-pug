open BsReactNative;

let styles =
  StyleSheet.create(
    Style.{
      "pug": style([width(Pt(50.)), height(Pt(50.))]),
      "pugShape":
        style([
          width(Pt(40.)),
          height(Pt(40.)),
          borderRadius(20.),
          top(Pt(0.)),
        ]),

      "textContainer":
        style([
          alignItems(Center),
          backgroundColor(String("transparent")),
        ]),
      "text": style([fontFamily("Fredoka One"), color(String("white"))]),
    },
  );

let component = ReasonReact.statelessComponent("Heart");

let make = (~style, ~heartAnimation, ~height, ~clickedBy, _children) => {
  ...component,
  didMount: _self => {
    Animated.start(
      Animated.timing(
        ~value=heartAnimation,
        ~toValue=`raw(height),
        ~duration=3000.0,
        (),
      ),
      (),
    );
  },
  render: _self => {
    <Animated.View style={Style.concat([styles##pug, style])}>
      <Image
        style=styles##pugShape
        source={
                 `URI(
                   Image.(
                     imageURISource(
                       ~uri=
                         "https://s3-us-west-2.amazonaws.com/jerk-twitch/pug.svg",
                       (),
                     )
                   ),
                 )
               }
      />
      <View style=styles##textContainer>
        <Text style=styles##text> {clickedBy |> ReasonReact.string} </Text>
      </View>
    </Animated.View>;
  },
};