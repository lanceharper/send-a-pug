open BsReactNative;

type action =
  | MouseEnter
  | MouseLeave;

type state = bool;

let component = ReasonReact.reducerComponent("Trigger");

let make = (~onPress, _children) => {
  ...component,
  initialState: () => false,
  reducer: (action, _state) =>
    switch (action) {
    | MouseEnter => ReasonReact.Update(true)
    | MouseLeave => ReasonReact.Update(false)
    },
  render: self =>
    <div
      onMouseEnter={_self => self.send(MouseEnter)}
      onMouseLeave={_self => self.send(MouseLeave)}>
      <View
        style=Style.(
          style([
            borderRadius(30.),
            borderTopWidth(1.),
            borderColor(self.state ? String("#eee") : String("#999")),
            height(Pt(60.)),
            width(Pt(60.)),
            backgroundColor(String("#rgba(122,103,238, 0.1)")),
            justifyContent(Center),
            alignItems(Center),
            padding(Pt(4.)),
            paddingTop(Pt(10.)),
          ])
        )>
        <TouchableWithoutFeedback onPress={_self => onPress(true)}>
          <Image
            resizeMode=`contain
            style=Style.(style([height(Pct(100.)), width(Pct(100.))]))
            source={
                     `URI(
                       Image.(
                         imageURISource(
                           ~uri=
                             "https://s3-us-west-2.amazonaws.com/jerk-twitch/pug2.svg",
                           (),
                         )
                       ),
                     )
                   }
          />
        </TouchableWithoutFeedback>
      </View>
    </div>,
};